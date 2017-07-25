package kubecollect

import (
	"draiosproto"
	"context"
	"github.com/gogo/protobuf/proto"
	"time"
	log "github.com/cihub/seelog"
	kubeclient "k8s.io/client-go/kubernetes"
	"k8s.io/client-go/tools/cache"
	v1meta "k8s.io/apimachinery/pkg/apis/meta/v1"
	"k8s.io/apimachinery/pkg/fields"
	"k8s.io/api/apps/v1beta1"
	//"k8s.io/apimachinery/pkg/labels"
	"k8s.io/api/core/v1"
)

// make this a library function?
func statefulSetEvent(ss *v1beta1.StatefulSet, eventType *draiosproto.CongroupEventType) (draiosproto.CongroupUpdateEvent) {
	return draiosproto.CongroupUpdateEvent {
		Type: eventType,
		Object: newStatefulSetCongroup(ss),
	}
}

func newStatefulSetCongroup(statefulSet *v1beta1.StatefulSet) (*draiosproto.ContainerGroup) {
	// Need a way to distinguish them
	// ... and make merging annotations+labels it a library function?
	//     should work on all v1.Object types
	tags := make(map[string]string)
	for k, v := range statefulSet.GetLabels() {
		tags["kubernetes.statefulset.label." + k] = v
	}
	tags["kubernetes.statefulset.name"] = statefulSet.GetName()

	ret := &draiosproto.ContainerGroup{
		Uid: &draiosproto.CongroupUid{
			Kind:proto.String("k8s_statefulset"),
			Id:proto.String(string(statefulSet.GetUID()))},
		Tags: tags,
		//Metrics: metrics,
	}
	AddNSParents(&ret.Parents, statefulSet.GetNamespace())
	AddPodChildrenFromOwnerRef(&ret.Children, statefulSet.ObjectMeta)
	AddServiceChildrenFromServiceName(&ret.Children, statefulSet.GetNamespace(), statefulSet.Spec.ServiceName)
	return ret
}

var statefulSetInf cache.SharedInformer

func AddStatefulSetParentsFromPod(parents *[]*draiosproto.CongroupUid, pod *v1.Pod) {
	for _, obj := range statefulSetInf.GetStore().List() {
		statefulSet := obj.(*v1beta1.StatefulSet)
		for _, owner := range pod.GetOwnerReferences() {
			if owner.UID == statefulSet.GetUID() {
				*parents = append(*parents, &draiosproto.CongroupUid{
					Kind:proto.String("k8s_statefulset"),
					Id:proto.String(string(statefulSet.GetUID()))})
			}
		}

	}
}

func AddStatefulSetParentsFromService(parents *[]*draiosproto.CongroupUid, service *v1.Service) {
	for _, obj := range statefulSetInf.GetStore().List() {
		statefulSet := obj.(*v1beta1.StatefulSet)
		if service.GetNamespace() == statefulSet.GetNamespace() && service.GetName() == statefulSet.Spec.ServiceName {
			*parents = append(*parents, &draiosproto.CongroupUid{
				Kind:proto.String("k8s_statefulset"),
				Id:proto.String(string(statefulSet.GetUID()))})
		}
	}
}

func WatchStatefulSets(ctx context.Context, kubeClient kubeclient.Interface, evtc chan<- draiosproto.CongroupUpdateEvent) cache.SharedInformer {
	log.Debugf("In WatchStatefulSets()")

	client := kubeClient.AppsV1beta1().RESTClient()
	lw := cache.NewListWatchFromClient(client, "StatefulSets", v1meta.NamespaceAll, fields.Everything())
	resyncPeriod := time.Duration(10) * time.Second;
	statefulSetInf = cache.NewSharedInformer(lw, &v1beta1.StatefulSet{}, resyncPeriod)

	statefulSetInf.AddEventHandler(
		cache.ResourceEventHandlerFuncs{
			AddFunc: func(obj interface{}) {
				//log.Debugf("AddFunc dumping StatefulSet: %v", obj.(*v1beta1.StatefulSet))
				evtc <- statefulSetEvent(obj.(*v1beta1.StatefulSet),
					draiosproto.CongroupEventType_ADDED.Enum())
			},
			UpdateFunc: func(oldObj, newObj interface{}) {
				oldStatefulSet := oldObj.(*v1beta1.StatefulSet)
				newStatefulSet := newObj.(*v1beta1.StatefulSet)
				if oldStatefulSet.GetResourceVersion() != newStatefulSet.GetResourceVersion() {
					//log.Debugf("UpdateFunc dumping StatefulSet oldStatefulSet %v", oldStatefulSet)
					//log.Debugf("UpdateFunc dumping StatefulSet newStatefulSet %v", newStatefulSet)
					evtc <- statefulSetEvent(newStatefulSet,
						draiosproto.CongroupEventType_UPDATED.Enum())
				}
			},
			DeleteFunc: func(obj interface{}) {
				//log.Debugf("DeleteFunc dumping StatefulSet: %v", obj.(*v1beta1.StatefulSet))
				evtc <- statefulSetEvent(obj.(*v1beta1.StatefulSet),
					draiosproto.CongroupEventType_REMOVED.Enum())
			},
		},
	)

	go statefulSetInf.Run(ctx.Done())

	return statefulSetInf
}
