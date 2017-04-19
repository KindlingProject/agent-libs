// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: sdc_internal.proto
#ifndef GRPC_sdc_5finternal_2eproto__INCLUDED
#define GRPC_sdc_5finternal_2eproto__INCLUDED

#include "sdc_internal.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace sdc_internal {

class CoInterface final {
 public:
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status PerformDockerCommand(::grpc::ClientContext* context, const ::sdc_internal::docker_command& request, ::sdc_internal::docker_command_result* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::sdc_internal::docker_command_result>> AsyncPerformDockerCommand(::grpc::ClientContext* context, const ::sdc_internal::docker_command& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::sdc_internal::docker_command_result>>(AsyncPerformDockerCommandRaw(context, request, cq));
    }
    virtual ::grpc::Status PerformPing(::grpc::ClientContext* context, const ::sdc_internal::ping& request, ::sdc_internal::pong* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::sdc_internal::pong>> AsyncPerformPing(::grpc::ClientContext* context, const ::sdc_internal::ping& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::sdc_internal::pong>>(AsyncPerformPingRaw(context, request, cq));
    }
    virtual ::grpc::Status PerformSwarmState(::grpc::ClientContext* context, const ::sdc_internal::swarm_state_command& request, ::sdc_internal::swarm_state_result* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::sdc_internal::swarm_state_result>> AsyncPerformSwarmState(::grpc::ClientContext* context, const ::sdc_internal::swarm_state_command& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::sdc_internal::swarm_state_result>>(AsyncPerformSwarmStateRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::sdc_internal::docker_command_result>* AsyncPerformDockerCommandRaw(::grpc::ClientContext* context, const ::sdc_internal::docker_command& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::sdc_internal::pong>* AsyncPerformPingRaw(::grpc::ClientContext* context, const ::sdc_internal::ping& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::sdc_internal::swarm_state_result>* AsyncPerformSwarmStateRaw(::grpc::ClientContext* context, const ::sdc_internal::swarm_state_command& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status PerformDockerCommand(::grpc::ClientContext* context, const ::sdc_internal::docker_command& request, ::sdc_internal::docker_command_result* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::sdc_internal::docker_command_result>> AsyncPerformDockerCommand(::grpc::ClientContext* context, const ::sdc_internal::docker_command& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::sdc_internal::docker_command_result>>(AsyncPerformDockerCommandRaw(context, request, cq));
    }
    ::grpc::Status PerformPing(::grpc::ClientContext* context, const ::sdc_internal::ping& request, ::sdc_internal::pong* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::sdc_internal::pong>> AsyncPerformPing(::grpc::ClientContext* context, const ::sdc_internal::ping& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::sdc_internal::pong>>(AsyncPerformPingRaw(context, request, cq));
    }
    ::grpc::Status PerformSwarmState(::grpc::ClientContext* context, const ::sdc_internal::swarm_state_command& request, ::sdc_internal::swarm_state_result* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::sdc_internal::swarm_state_result>> AsyncPerformSwarmState(::grpc::ClientContext* context, const ::sdc_internal::swarm_state_command& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::sdc_internal::swarm_state_result>>(AsyncPerformSwarmStateRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::sdc_internal::docker_command_result>* AsyncPerformDockerCommandRaw(::grpc::ClientContext* context, const ::sdc_internal::docker_command& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::sdc_internal::pong>* AsyncPerformPingRaw(::grpc::ClientContext* context, const ::sdc_internal::ping& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::sdc_internal::swarm_state_result>* AsyncPerformSwarmStateRaw(::grpc::ClientContext* context, const ::sdc_internal::swarm_state_command& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::RpcMethod rpcmethod_PerformDockerCommand_;
    const ::grpc::RpcMethod rpcmethod_PerformPing_;
    const ::grpc::RpcMethod rpcmethod_PerformSwarmState_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status PerformDockerCommand(::grpc::ServerContext* context, const ::sdc_internal::docker_command* request, ::sdc_internal::docker_command_result* response);
    virtual ::grpc::Status PerformPing(::grpc::ServerContext* context, const ::sdc_internal::ping* request, ::sdc_internal::pong* response);
    virtual ::grpc::Status PerformSwarmState(::grpc::ServerContext* context, const ::sdc_internal::swarm_state_command* request, ::sdc_internal::swarm_state_result* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_PerformDockerCommand : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_PerformDockerCommand() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_PerformDockerCommand() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PerformDockerCommand(::grpc::ServerContext* context, const ::sdc_internal::docker_command* request, ::sdc_internal::docker_command_result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestPerformDockerCommand(::grpc::ServerContext* context, ::sdc_internal::docker_command* request, ::grpc::ServerAsyncResponseWriter< ::sdc_internal::docker_command_result>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_PerformPing : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_PerformPing() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_PerformPing() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PerformPing(::grpc::ServerContext* context, const ::sdc_internal::ping* request, ::sdc_internal::pong* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestPerformPing(::grpc::ServerContext* context, ::sdc_internal::ping* request, ::grpc::ServerAsyncResponseWriter< ::sdc_internal::pong>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_PerformSwarmState : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_PerformSwarmState() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_PerformSwarmState() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PerformSwarmState(::grpc::ServerContext* context, const ::sdc_internal::swarm_state_command* request, ::sdc_internal::swarm_state_result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestPerformSwarmState(::grpc::ServerContext* context, ::sdc_internal::swarm_state_command* request, ::grpc::ServerAsyncResponseWriter< ::sdc_internal::swarm_state_result>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_PerformDockerCommand<WithAsyncMethod_PerformPing<WithAsyncMethod_PerformSwarmState<Service > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_PerformDockerCommand : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_PerformDockerCommand() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_PerformDockerCommand() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PerformDockerCommand(::grpc::ServerContext* context, const ::sdc_internal::docker_command* request, ::sdc_internal::docker_command_result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_PerformPing : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_PerformPing() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_PerformPing() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PerformPing(::grpc::ServerContext* context, const ::sdc_internal::ping* request, ::sdc_internal::pong* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_PerformSwarmState : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_PerformSwarmState() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_PerformSwarmState() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PerformSwarmState(::grpc::ServerContext* context, const ::sdc_internal::swarm_state_command* request, ::sdc_internal::swarm_state_result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_PerformDockerCommand : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_PerformDockerCommand() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::StreamedUnaryHandler< ::sdc_internal::docker_command, ::sdc_internal::docker_command_result>(std::bind(&WithStreamedUnaryMethod_PerformDockerCommand<BaseClass>::StreamedPerformDockerCommand, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_PerformDockerCommand() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status PerformDockerCommand(::grpc::ServerContext* context, const ::sdc_internal::docker_command* request, ::sdc_internal::docker_command_result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedPerformDockerCommand(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::sdc_internal::docker_command,::sdc_internal::docker_command_result>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_PerformPing : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_PerformPing() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::StreamedUnaryHandler< ::sdc_internal::ping, ::sdc_internal::pong>(std::bind(&WithStreamedUnaryMethod_PerformPing<BaseClass>::StreamedPerformPing, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_PerformPing() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status PerformPing(::grpc::ServerContext* context, const ::sdc_internal::ping* request, ::sdc_internal::pong* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedPerformPing(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::sdc_internal::ping,::sdc_internal::pong>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_PerformSwarmState : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_PerformSwarmState() {
      ::grpc::Service::MarkMethodStreamed(2,
        new ::grpc::StreamedUnaryHandler< ::sdc_internal::swarm_state_command, ::sdc_internal::swarm_state_result>(std::bind(&WithStreamedUnaryMethod_PerformSwarmState<BaseClass>::StreamedPerformSwarmState, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_PerformSwarmState() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status PerformSwarmState(::grpc::ServerContext* context, const ::sdc_internal::swarm_state_command* request, ::sdc_internal::swarm_state_result* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedPerformSwarmState(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::sdc_internal::swarm_state_command,::sdc_internal::swarm_state_result>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_PerformDockerCommand<WithStreamedUnaryMethod_PerformPing<WithStreamedUnaryMethod_PerformSwarmState<Service > > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_PerformDockerCommand<WithStreamedUnaryMethod_PerformPing<WithStreamedUnaryMethod_PerformSwarmState<Service > > > StreamedService;
};

}  // namespace sdc_internal


#endif  // GRPC_sdc_5finternal_2eproto__INCLUDED
