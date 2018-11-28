#include "pch.h"

#include "Tools.h"

#include "BaseChatCommand.h"
#include "UserEntity.h"
#include "ChatCommandResolver.h"


namespace PocoChatGeneral
{
  std::shared_ptr<ChatCommandResolver> ChatCommandResolver::Create(std::function<std::unordered_map<std::string, std::shared_ptr<BaseChatCommand>>(const std::shared_ptr<ChatCommandResolver>&)> command_generator)
  {
    auto resolver = std::shared_ptr<ChatCommandResolver>{ new ChatCommandResolver() };
    resolver->initCommands(command_generator(resolver));
    return resolver;
  }

  void ChatCommandResolver::initCommands(std::unordered_map<std::string, std::shared_ptr<BaseChatCommand>>&& commands)
  {
    commands_ = std::move(commands);
  }

  std::string ChatCommandResolver::ExtractCommand(const std::string& data)
  {
    return BaseChatCommand::CommandNetworkData::ExtractCommand(data);
  }

  bool ChatCommandResolver::processClient(UserEntity& command_source, const std::string& command, const std::string& args) const
  {
    auto found_command = commands_.find(command);
    return found_command == commands_.end() ? UnknownCommand(command) : knownClientCommand(command_source, found_command->second, args);
  }

  bool ChatCommandResolver::processServer(UserEntity& command_source, const std::string& command, const std::string& args) const
  {
    auto found_command = commands_.find(command);
    return found_command == commands_.end() ? UnknownCommand(command) : knownServerCommand(command_source, found_command->second, args);
  }

  bool ChatCommandResolver::UnknownCommand(const std::string& command)
  {
    PocoChatGeneral::debug_message("Called unknown command `", command, "`");

    return true;
  }

  bool ChatCommandResolver::knownServerCommand(UserEntity& command_source, const std::shared_ptr<BaseChatCommand>& called_command, const std::string& args) const
  {
    PocoChatGeneral::debug_message("Received from `", command_source.name(), "` command `", called_command->name(), "` with args: ", args);

    return called_command->handleServer(command_source, args);
  }

  bool ChatCommandResolver::knownClientCommand(UserEntity& command_source, const std::shared_ptr<BaseChatCommand>& called_command, const std::string& args) const
  {
    PocoChatGeneral::debug_message("Called `", called_command->name(), "` with args: ", args);

    return called_command->handleClient(command_source, args);
  }

}
