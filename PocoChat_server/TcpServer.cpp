#include "pch.h"

#include <string>

#include "Definition.h"
#include "Tools.h"

#include "ServerCommandResolver.h"
#include "ConnectionFactory.h"
#include "TcpServer.h"


TcpServer::TcpServer(
  const std::shared_ptr<UserEntities>& user_entities,
  const PocoChatGeneral::ServerInformation& kServerInformation,
  const std::shared_ptr<PocoChatGeneral::ChatCommandResolver>& client_command_resolver,
  std::shared_ptr<ServerCommandResolver>&& server_command_resolver) :
  server_(new ConnectionFactory(user_entities, client_command_resolver), PocoChatGeneral::create_socket<Poco::Net::ServerSocket>(kServerInformation)),
  command_resolver_(std::move(server_command_resolver))
{
}

void TcpServer::run()
{
  server_.start();
  PocoChatGeneral::info_message("Server's working");

  resolveCommands();
}

void TcpServer::resolveCommands()
{
  PocoChatGeneral::parse_input([this](const std::string& command, const std::string& args)
  {
    return this->command_resolver_->resolve(command, args);
  });
}

TcpServer::~TcpServer()
{
  server_.stop();
  PocoChatGeneral::info_message("Server's stopped");
}
