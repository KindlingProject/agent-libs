#!/usr/bin/env python
import sys
import time
import draios_pb2
import zlib
import os
import errno
from protobuf_to_dict import protobuf_to_dict
import simplejson as json
from jq import jq
from google.protobuf.text_format import Merge as parse_text_protobuf
from google.protobuf.text_format import MessageToString as serialize_text_protobuf
import os.path
import subprocess
import argparse
import struct
import socket
import atexit
from datetime import datetime

# this class parses text dumped protobuf from the agent
# they can be enabled with "metricsfile: { location: metrics }"
# on dragent.yaml
# it works if the agent is still running and will report data
# continously using `tail -f`
# TODO: it needs improvements tobe able to parse protobufs coming
#       from protoparser
class MetricsFile(object):
  def __init__(self, path, tail=False):
    if tail:
      self._tail = subprocess.Popen(["tail", "-f", path], stdout=subprocess.PIPE)
      atexit.register(self.close_tail)
      self._file = self._tail.stdout
    else:
      self._file = open(path)

    # Detect if it's a "metrics {" or "timestamp_ns" file
    self._last_line = self._file.readline()
    while True:
      if self._last_line.startswith("timestamp_ns"):
        self.proto_start = "timestamp_ns"
        break
      elif self._last_line.startswith("metrics {"):
        self.proto_start = "metrics {"
        break
      else:
        self._last_line = self._file.readline()

  def next(self):
    ascii_repr = self._last_line
    self._last_line = self._file.readline()
    if len(self._last_line) == 0:
      raise StopIteration()
    while not self._last_line.startswith(self.proto_start):
      ascii_repr += self._last_line
      self._last_line = self._file.readline()
      if len(self._last_line) == 0:
        raise StopIteration()
    # Trim "metrics {"
    if self.proto_start == "metrics {":
      ascii_repr = "\n".join(ascii_repr.split("\n")[1:-2])
    metrics = draios_pb2.metrics()
    parse_text_protobuf(ascii_repr, metrics)
    return metrics

  def __iter__(self):
    return self

  def close_tail(self):
    self._tail.kill()

parser = argparse.ArgumentParser(description="Analyze protobufs using JQ filters")
parser.add_argument("--follow", dest="follow", required=False, default=False, action='store_true', help="Follow the file as tail -f does")
parser.add_argument("--binary", dest="binary", required=False, default=False, action='store_true', help="path is a binary file")
parser.add_argument("--reorder", dest="reorder", required=False, default=False, action="store_true", help="reorder metrics by timestamp")
parser.add_argument("--jq-filter", type=str, required=False, help="JQ filter to use")
parser.add_argument("--filter", type=str, required=False, help="Native functions available")
parser.add_argument("path", type=str, help="File to parse")
args = parser.parse_args()

def walk_protos(path, ext="dam"):
  for root, dirs, files in os.walk(path, topdown=False):
    for name in files:
      if name.endswith(ext):
        fullpath = os.path.join(root, name)
        analyze_proto(fullpath)

kubernetes_delegated_nodes = set()
running_containers = set()
kubernetes_containers = set()
containers_by_id = {}
deployments = set()
k8s_nodes = set()
nodes = set()

def kubernetes_check(m):
  if "kubernetes" in m:
    kubernetes_delegated_nodes.add( ( m["machine_id"], m["hostinfo"]["hostname"]))
    for pod in m["kubernetes"]["pods"]:
      for c in pod["container_ids"]:
        container_id = c[9:12+9]
        kubernetes_containers.add(container_id)
    for deployment in m["kubernetes"]["deployments"]:
      deployments.add(deployment["common"]["name"])
    for node in m["kubernetes"]["nodes"]:
      k8s_nodes.add( (node["common"]["uid"], node["common"]["name"]))
  if "containers" in m:
    for c in m["containers"]:
      running_containers.add(c["id"])
      containers_by_id[c['id']] = ( c["name"] )
  nodes.add(( m["machine_id"], m["hostinfo"]["hostname"]))

def print_kubernetes_summary():
  print "Delegated: %s" % str(kubernetes_delegated_nodes)
  print "matching containers=%d" % len(running_containers.intersection(kubernetes_containers))
  for cid in running_containers.intersection(kubernetes_containers):
    print "name=%s" % containers_by_id[cid]
  print "only on k8s containers=%d" % len(kubernetes_containers.difference(running_containers))
  print "only running containers=%d" % len(running_containers.difference(kubernetes_containers))
  for cid in running_containers.difference(kubernetes_containers):
    print "name=%s" % containers_by_id[cid]
  print "deployments=%s" % str(deployments) 
  print "k8s_nodes=%d\n%s" % (len(k8s_nodes), str(k8s_nodes))
  print "nodes=%d\n%s" % (len(k8s_nodes), str(nodes))

def create_filter():
  if args.filter:
    return globals()[args.filter]
  elif args.jq_filter:
    jq_filter = jq(args.jq_filter)
    return lambda m: jq_filter.transform(m, multiple_output=True)
  else:
    return lambda m: m

def analyze_proto(path):
  filter_f = create_filter()
  if path.endswith("dam") or args.binary:
    with open(path, "rb") as f:
      f.seek(2)
      metrics = draios_pb2.metrics.FromString(f.read())
      process_metrics(metrics, filter_f)
  else:
      if args.reorder:
        ml = [ metrics for metrics in MetricsFile(path)]
        ml.sort(key=lambda m: m.timestamp_ns)
        for m in ml:
          process_metrics(m, filter_f)
      else:
        for metrics in MetricsFile(path, tail=args.follow):
          process_metrics(metrics, filter_f)

def process_metrics(metrics, filter_f):
  ts = datetime.fromtimestamp(metrics.timestamp_ns/1000000000)
  print("###### sample ts=%s ######" % ts.strftime("%Y-%m-%d %H:%M:%S"))
  metrics_d = protobuf_to_dict(metrics)
  metrics_j = filter_f(metrics_d)
  print(json.dumps(metrics_j, indent=2))
  print("\n")

 
print "Running with args: %s" % repr(args)

if args.path == "last":
  metricFiles = [ p for p in os.listdir("/opt/draios/metrics/") if p.endswith(".dams") ]
  path = os.path.join("/opt/draios/metrics/", metricFiles[-1])
else:
  path = args.path

# text files
#
def main():
  if os.path.isdir(path):
    walk_protos(path)
  else:
    analyze_proto(path)
  print_kubernetes_summary()
if __name__ == "__main__":
  try:
    main()
  except KeyboardInterrupt:
    print("Ctrl-C closing..")
