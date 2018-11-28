#include "pch.h"

#include "Tools.h"

#include "ChatCommandResolver.h"
#include "ClientCommandResolver.h"


ClientCommandResolver::ClientCommandResolver(const std::unordered_map<std::string, std::string>& key_mapping, std::shared_ptr<PocoChatGeneral::ChatCommandResolver>&& command_resolver) :
  key_mapping_(key_mapping), command_resolver_(std::move(command_resolver))
{
}

bool ClientCommandResolver::resolve(PocoChatGeneral::UserEntity& command_source, const std::string& command, const std::string& args) const
{
  auto mapped_key = key_mapping_.find(command);
  return mapped_key == key_mapping_.end() ? UnknownCommand() : command_resolver_->processClient(command_source, mapped_key->second, args);
}

bool ClientCommandResolver::UnknownCommand()
{
  PocoChatGeneral::debug_message("Called unknown command");

  return true;
}

const std::shared_ptr<PocoChatGeneral::ChatCommandResolver>& ClientCommandResolver::chatResolver() const
{
  return command_resolver_;
}