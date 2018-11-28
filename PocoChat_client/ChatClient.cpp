#include "pch.h"

#include "Poco/Exception.h"
#include "Poco/Net/NetException.h"

#include "UserEntity.h"
#include "Definition.h"
#include "ChatCommandResolver.h"
#include "Tools.h"

#include "ClientUserEntity.h"
#include "ClientCommandResolver.h"
#include "ChatClient.h"


ChatClient::ChatClient(const PocoChatGeneral::ServerInformation& kServerInformation, std::unique_ptr<ClientCommandResolver>&& command_resolver) :
  socket_(PocoChatGeneral::create_socket<Poco::Net::StreamSocket>(kServerInformation)), user_entity_(std::make_unique<ClientUserEntity>(socket_)),
  command_resolver_(std::move(command_resolver)), is_working_(true), async_listener_([this]() {this->received_async(); })
{
  PocoChatGeneral::info_message("Connected to server");
}

ChatClient::~ChatClient()
{
  socket_.shutdown();
  is_working_ = false;
  async_listener_.join();
  PocoChatGeneral::info_message("Disconnected from server");
}

void ChatClient::received_async()
{
  while (is_working_)
  {
    auto& command_resolver = *command_resolver_->chatResolver();
    try
    {
      PocoChatGeneral::listen_input(*user_entity_->network_communicator(), [this, command_resolver](const std::string& message)
      {
        return command_resolver.processClient(*this->user_entity_, command_resolver.ExtractCommand(message), message);
      });
    }
    catch (const Poco::TimeoutException&)
    {
    }
    catch (const Poco::Net::NetException& e)
    {
      PocoChatGeneral::debug_message("Socket exception: ", e.what());
      is_working_ = false;
    }
  }
}

void ChatClient::run()
{
  PocoChatGeneral::parse_input([this](const std::string& command, const std::string& args)
  {
    return this->is_working_ && this->command_resolver_->resolve(*this->user_entity_, command, args);
  });
}
