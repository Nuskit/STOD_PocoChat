#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <memory>

#include "Poco/Net/TCPServerConnection.h"

namespace PocoChatGeneral
{
  class ChatCommandResolver;
}

class ServerUserEntity;

class ServerConnection: public Poco::Net::TCPServerConnection
{
public:
  ServerConnection(const Poco::Net::StreamSocket& current_socket, const std::shared_ptr<ServerUserEntity>& user_entity, const std::shared_ptr<PocoChatGeneral::ChatCommandResolver>& command_resolver);
  virtual ~ServerConnection();

  virtual void run() override;

private:
  std::shared_ptr<PocoChatGeneral::ChatCommandResolver> command_resolver_;
  std::shared_ptr<ServerUserEntity> user_entity_;
};


#endif 
