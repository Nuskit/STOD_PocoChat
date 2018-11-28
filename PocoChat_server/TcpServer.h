#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <memory>

#include "Poco/Net/TCPServer.h"

#include "Definition.h"


namespace PocoChatGeneral
{
  struct ServerInformation;
  class ChatCommandResolver;
}

class ServerCommandResolver;
class UserEntities;

class TcpServer
{
public:
  TcpServer(
    const std::shared_ptr<UserEntities>& user_entities,
    const PocoChatGeneral::ServerInformation& kServerInformation,
    const std::shared_ptr<PocoChatGeneral::ChatCommandResolver>& client_command_resolver,
    std::shared_ptr<ServerCommandResolver>&& server_command_resolver);

  TcpServer(const TcpServer&) = delete;
  TcpServer(TcpServer&&) = delete;
  TcpServer& operator=(const TcpServer&) = delete;
  TcpServer& operator=(TcpServer&&) = delete;
  ~TcpServer();

  void run();

private:
  void resolveCommands();

  Poco::Net::TCPServer server_;
  std::shared_ptr<ServerCommandResolver> command_resolver_;
};

#endif
