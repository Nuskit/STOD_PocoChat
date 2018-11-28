#ifndef CHAT_COMMAND_RESOLVER_H
#define CHAT_COMMAND_RESOLVER_H

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>


namespace PocoChatGeneral
{
  class BaseChatCommand;
  class UserEntity;

  class ChatCommandResolver
  {
  public:
    static std::shared_ptr<ChatCommandResolver> Create(std::function<std::unordered_map<std::string, std::shared_ptr<BaseChatCommand>>(const std::shared_ptr<ChatCommandResolver>&)> command_generator);

    ChatCommandResolver(const ChatCommandResolver&) = default;
    ChatCommandResolver(ChatCommandResolver&&) = default;
    ChatCommandResolver& operator=(const ChatCommandResolver&) = default;
    ChatCommandResolver& operator=(ChatCommandResolver&&) = default;
    ~ChatCommandResolver() = default;

    static std::string ExtractCommand(const std::string& data);
    bool processClient(UserEntity& command_source, const std::string& command, const std::string& args) const;
    bool processServer(UserEntity& command_source, const std::string& command, const std::string& args) const;

  private:
    ChatCommandResolver() = default;
    void initCommands(std::unordered_map<std::string, std::shared_ptr<BaseChatCommand>>&& commands);

    static bool UnknownCommand(const std::string& command);
    bool knownServerCommand(UserEntity& command_source, const std::shared_ptr<BaseChatCommand>& called_command, const std::string& args) const;
    bool knownClientCommand(UserEntity& command_source, const std::shared_ptr<BaseChatCommand>& called_command, const std::string& args) const;

    std::unordered_map<std::string, std::shared_ptr<BaseChatCommand>> commands_;
  };
}

#endif
