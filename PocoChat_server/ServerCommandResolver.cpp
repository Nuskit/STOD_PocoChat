#include "pch.h"

#include "Tools.h"

#include "BaseServerCommand.h"
#include "ServerCommandResolver.h"


ServerCommandResolver::ServerCommandResolver(std::unordered_map<std::string, std::shared_ptr<BaseServerCommand>>&& commands) : 
  commands_(std::move(commands))
{
}

bool ServerCommandResolver::resolve(const std::string& command, const std::string& args)
{
  auto found_command = commands_.find(command);
  return found_command == commands_.end() ? UnknownCommand() : knownCommand(found_command->second, args);
}

bool ServerCommandResolver::UnknownCommand()
{
  PocoChatGeneral::debug_message("Called unknown command");

  return true;
}

bool ServerCommandResolver::knownCommand(const std::shared_ptr<BaseServerCommand>& called_command, const std::string& args)
{
  PocoChatGeneral::debug_message("Called ", called_command->name(), " with args: ", args);

  return called_command->handle(args);
}
