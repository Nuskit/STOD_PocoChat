#include "pch.h"

#include "ServerConnection.h"
#include "UserEntities.h"
#include "ServerUserEntity.h"
#include "ConnectionFactory.h"


ConnectionFactory::ConnectionFactory(
  const std::shared_ptr<UserEntities>& user_entities,
  const std::shared_ptr<PocoChatGeneral::ChatCommandResolver>& command_resolver) :
  Poco::Net::TCPServerConnectionFactory(), command_resolver_(command_resolver), user_entities_(user_entities)
{
}

Poco::Net::TCPServerConnection * ConnectionFactory::createConnection(const Poco::Net::StreamSocket& socket)
{
  return new ServerConnection(socket, getUserByConnection(socket), command_resolver_);
}

std::shared_ptr<ServerUserEntity> ConnectionFactory::getUserByConnection(const Poco::Net::StreamSocket& socket)
{
  std::string user_identifier = socket.peerAddress().toString();
  if (!user_entities_->existUser(user_identifier))
  {
    user_entities_->registerUser(user_identifier, user_entities_->CreateUser(socket));
  }
  
  return user_entities_->getUser(user_identifier);
}
