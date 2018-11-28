#ifndef CONNECTION_FACTORY_H
#define CONNECTION_FACTORY_H

#include <memory>

#include "Poco/Net/TCPServerConnectionFactory.h"


namespace PocoChatGeneral
{
  class ChatCommandResolver;
  
}

class UserEntities;
class ServerUserEntity;

class ConnectionFactory: public Poco::Net::TCPServerConnectionFactory
{
public:
  explicit ConnectionFactory(const std::shared_ptr<UserEntities>& user_entities, const std::shared_ptr<PocoChatGeneral::ChatCommandResolver>& command_resolver);

  virtual Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket) override;

private:
  std::shared_ptr<ServerUserEntity> getUserByConnection(const Poco::Net::StreamSocket& socket);

  std::shared_ptr<PocoChatGeneral::ChatCommandResolver> command_resolver_;
  std::shared_ptr<UserEntities> user_entities_;
};

#endif
