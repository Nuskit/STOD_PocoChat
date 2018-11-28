#ifndef CLIENT_COMMAND_RESOLVER_H
#define CLIENT_COMMAND_RESOLVER_H

#include <memory>
#include <unordered_map>


namespace PocoChatGeneral
{
  class ChatCommandResolver;
  class UserEntity;
}

class ClientCommandResolver
{
public:
  ClientCommandResolver(const std::unordered_map<std::string, std::string>& key_mapping, std::shared_ptr<PocoChatGeneral::ChatCommandResolver>&& command_resolver);
  ClientCommandResolver(const ClientCommandResolver&) = delete;
  ClientCommandResolver(ClientCommandResolver&&) = delete;
  ClientCommandResolver& operator=(const ClientCommandResolver&) = delete;
  ClientCommandResolver& operator=(ClientCommandResolver&&) = delete;
  ~ClientCommandResolver() = default;

  bool resolve(PocoChatGeneral::UserEntity& command_source, const std::string& command, const std::string& args) const;

  const std::shared_ptr<PocoChatGeneral::ChatCommandResolver>& chatResolver() const;
private:
  static bool UnknownCommand();
  std::unordered_map<std::string, std::string> key_mapping_;
  std::shared_ptr<PocoChatGeneral::ChatCommandResolver> command_resolver_;
};


#endif
