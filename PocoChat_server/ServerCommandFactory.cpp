#include "pch.h"

#include <unordered_map>
#include <string>

#include "ExitServerCommand.h"
#include "ServerCommandResolver.h"
#include "ServerCommandFactory.h"

class BaseServerCommand;

std::unordered_map<std::string, std::shared_ptr<BaseServerCommand>> server_commands()
{
  return { std::make_pair("/exit", std::make_shared<ExitServerCommand>()) };
}


std::unique_ptr<ServerCommandResolver> create_server_resolver()
{
  return std::make_unique<ServerCommandResolver>(server_commands());
}
