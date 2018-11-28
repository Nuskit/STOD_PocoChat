#ifndef SERVER_COMMAND_RESOLVER_H
#define SERVER_COMMAND_RESOLVER_H

#include <unordered_map>
#include <memory>
#include <string>


class BaseServerCommand;

class ServerCommandResolver
{
public:
  explicit ServerCommandResolver(std::unordered_map<std::string, std::shared_ptr<BaseServerCommand>>&& commands);
  ServerCommandResolver(const ServerCommandResolver&) = default;
  ServerCommandResolver(ServerCommandResolver&&) = default;
  ServerCommandResolver& operator=(const ServerCommandResolver&) = default;
  ServerCommandResolver& operator=(ServerCommandResolver&&) = default;
  ~ServerCommandResolver() = default;

  bool resolve(const std::string& command, const std::string& args);

private:
  static bool UnknownCommand();
  bool knownCommand(const std::shared_ptr<BaseServerCommand>& called_command, const std::string& args);

  std::unordered_map<std::string, std::shared_ptr<BaseServerCommand>> commands_;
};

#endif
