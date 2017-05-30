#include <Poco/File.h>

// From sysdig, for g_logger
#include "sinsp.h"
#include "sinsp_int.h"
#include "logger.h"

#include "analyzer_utils.h"
#include "coclient.h"

using namespace std;

std::string coclient::default_domain_sock = string("/opt/draios/run/cointerface.sock");

coclient::coclient():
	m_domain_sock(default_domain_sock),
	m_outstanding_swarm_state(false)
{
	m_print.SetSingleLineMode(true);
}

coclient::~coclient()
{
}

void coclient::connect()
{
	m_stub = sdc_internal::CoInterface::NewStub(grpc::CreateChannel(string("unix:") + m_domain_sock,
								grpc::InsecureChannelCredentials()));
}

void coclient::prepare(google::protobuf::Message *request_msg,
		       sdc_internal::cointerface_message_type msg_type,
		       response_cb_t response_cb)
{
	if(!m_stub)
	{
		connect();
	}

	string tmp;
	// FIXME: disabled because causes segfault on tests
	// m_print.PrintToString(*request_msg, &tmp);
	g_logger.log("Sending message to cointerface: " + tmp, sinsp_logger::SEV_DEBUG);

	call_context *call = new call_context();

	call->msg_type = msg_type;
	call->response_cb = response_cb;

	// Perform the (async) rpc
	// This only works because we only have a single rpc function
	// for a given request message type.
	switch(msg_type) {
		sdc_internal::ping *ping;
		sdc_internal::docker_command *docker_command;
		sdc_internal::swarm_state_command *sscmd;

	case sdc_internal::PING:
		// Start the rpc call and have the pong reader read the response when
		// it's ready.
		ping = static_cast<sdc_internal::ping *>(request_msg);
		call->pong_reader = m_stub->AsyncPerformPing(&call->ctx, *ping, &m_cq);

		// Tell the pong reader to write the response into the
		// response message, update status with whether or not the
		// rpc could be performed, and tag the rpc with a tag
		// that is the address of the call struct.
		call->response_msg = make_unique<sdc_internal::pong>();
		call->pong_reader->Finish(static_cast<sdc_internal::pong *>(call->response_msg.get()), &call->status, (void*)call);
		break;

	case sdc_internal::SWARM_STATE_COMMAND:
		sscmd = static_cast<sdc_internal::swarm_state_command *>(request_msg);
		call->swarm_state_reader = m_stub->AsyncPerformSwarmState(&call->ctx, *sscmd, &m_cq);

		call->response_msg = make_unique<sdc_internal::swarm_state_result>();
		call->swarm_state_reader->Finish(static_cast<sdc_internal::swarm_state_result *>(call->response_msg.get()), &call->status, (void*)call);
		break;

	case sdc_internal::DOCKER_COMMAND:
                // Start the rpc call and have the docker_cmd_result reader read the response when
                // it's ready.
		docker_command = static_cast<sdc_internal::docker_command *>(request_msg);
		call->docker_cmd_result_reader = m_stub->AsyncPerformDockerCommand(&call->ctx, *docker_command, &m_cq);

		// Tell the reader to write the response into the
		// response message, update status with whether or not
		// the rpc could be performed, and tag the rpc with a
		// tag that is the address of the call struct.
		call->response_msg = make_unique<sdc_internal::docker_command_result>();
		call->docker_cmd_result_reader->Finish(static_cast<sdc_internal::docker_command_result *>(call->response_msg.get()),
						       &call->status, (void*)call);

		break;

	default:
		g_logger.log("Unknown message type " + to_string(msg_type), sinsp_logger::SEV_ERROR);
		break;
	}
}

void coclient::next(uint32_t wait_ms)
{
	void *tag;
	bool updates_ok;
	grpc::CompletionQueue::NextStatus status;
	gpr_timespec deadline = gpr_time_from_millis(wait_ms, GPR_TIMESPAN);

	status = m_cq.AsyncNext(&tag, &updates_ok, deadline);

	if(status == grpc::CompletionQueue::SHUTDOWN)
	{
		g_logger.log("cointerface process shut down, disconnecting", sinsp_logger::SEV_ERROR);
		m_stub = NULL;
		m_outstanding_swarm_state = false;
		return;
	}
	else if(status == grpc::CompletionQueue::TIMEOUT)
	{
		return;
	}

	call_context *call = static_cast<call_context *>(tag);

	if(call->msg_type == sdc_internal::SWARM_STATE_COMMAND)
	{
		m_outstanding_swarm_state = false;
	}

	if(!updates_ok) {
		g_logger.log("cointerface RPC could not be scheduled successfully", sinsp_logger::SEV_ERROR);
		m_stub = NULL;
		return;
	}

	if(call->status.ok()) {
		string tmp;
		// FIXME: disabled because causes segfault on tests
		// m_print.PrintToString(*(call->response_msg), &tmp);

		g_logger.log("Got response from cointerface: " + tmp, sinsp_logger::SEV_DEBUG);

	} else {
		g_logger.log("cointerface rpc failed", sinsp_logger::SEV_DEBUG);
	}

	call->response_cb(call->status.ok(), call->response_msg.get());

	delete call;
}

void coclient::set_domain_sock(std::string &domain_sock)
{
	m_domain_sock = domain_sock;
}

void coclient::cleanup()
{
	Poco::File f(default_domain_sock);
	if(f.exists())
	{
		f.remove();
	}
}

void coclient::ping(int64_t token, response_cb_t response_cb)
{
	sdc_internal::ping ping;

	ping.set_token(token);

	prepare(&ping, sdc_internal::PING, response_cb);
}

void coclient::perform_docker_cmd(sdc_internal::docker_cmd_type cmd,
				  const string &container_id, response_cb_t response_cb)
{
	sdc_internal::docker_command docker_cmd;

	docker_cmd.set_cmd(cmd);
	docker_cmd.set_container_id(container_id);

	prepare(&docker_cmd, sdc_internal::DOCKER_COMMAND, response_cb);
}

void coclient::get_swarm_state(response_cb_t response_cb)
{
	if(m_outstanding_swarm_state)
	{
		g_logger.log("Swarm State requested while still pending", sinsp_logger::SEV_WARNING);
		return;
	}
	m_outstanding_swarm_state = true;

	sdc_internal::swarm_state_command cmd;
	prepare(&cmd, sdc_internal::SWARM_STATE_COMMAND, response_cb);
}
