#include "pch.h"

#include "Poco/Net/NetException.h"

#include "ChatCommandResolver.h"
#include "NetworkCommunicator.h"
#include "Tools.h"

#include "ServerUserEntity.h"
#include "ServerConnection.h"


ServerConnection::ServerConnection(const Poco::Net::StreamSocket& current_socket, const std::shared_ptr<ServerUserEntity>& user_entity, const std::shared_ptr<PocoChatGeneral::ChatCommandResolver>& command_resolver) :
  Poco::Net::TCPServerConnection(current_socket), command_resolver_(command_resolver), user_entity_(user_entity)
{
  user_entity_->network_communicator()->changeSocket(socket());
  user_entity_->connected();
}

void ServerConnection::run()
{
  try
  {
    PocoChatGeneral::listen_input(*user_entity_->network_communicator(), [this](const std::string& message)
    {
      return this->command_resolver_->processServer(*this->user_entity_, this->command_resolver_->ExtractCommand(message), message);
    });
  }
  catch (const Poco::Net::NetException& e)
  {
    PocoChatGeneral::debug_message("Net exception: ", e.what());
  }
}

ServerConnection::~ServerConnection()
{
  user_entity_->disconnected();
}
