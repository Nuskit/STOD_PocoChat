#include "pch.h"

#include <unordered_map>
#include <string>
#include <functional>

#include "ChatCommandResolver.h"
#include "DisconnectChatCommand.h"
#include "RequestOnlineUserChatCommand.h"
#include "ResponseOnlineUserChatCommand.h"
#include "RequestMessageChatCommand.h"
#include "ResponseMessageChatCommand.h"
#include "ChatCommandFactory.h"


namespace PocoChatGeneral
{
  class BaseChatCommand;

  template<class T, class... Args>
  static auto command_pair(Args&&...args)
  {
    return std::make_pair(T::command_index, std::make_shared<T>(std::forward<Args>(args)...));
  }

  std::unordered_map<std::string, std::shared_ptr<BaseChatCommand>> chat_commands(const std::shared_ptr<TaskManager>& task_manager, 
    std::function<std::vector<std::string>(int)>&& online_user_generator, 
    std::function<std::shared_ptr<UserEntity>(std::string)> user_searcher,
    const std::shared_ptr<ChatCommandResolver>& resolver)
  {
    return
    {
      command_pair<DisconnectChatCommand>(),
      command_pair<RequestOnlineUserChatCommand>(task_manager, [resolver](UserEntity& user_entity, const std::string& args)
      {
        resolver->processServer(user_entity, ResponseOnlineUserChatCommand::command_index, args);
      }),
      command_pair<ResponseOnlineUserChatCommand>(std::move(online_user_generator)),
      command_pair<RequestMessageChatCommand>(task_manager, [resolver](UserEntity& user_entity, const std::string& args)
      {
        resolver->processServer(user_entity, ResponseMessageChatCommand::command_index, args);
      }, std::move(user_searcher)),
      command_pair<ResponseMessageChatCommand>()
    };
  }

  std::shared_ptr<ChatCommandResolver> create_resolver(const std::shared_ptr<TaskManager>& task_manager,
    std::function<std::vector<std::string>(int)> online_user_generator,
    std::function<std::shared_ptr<UserEntity>(const std::string&)> user_searcher)
  {
    return ChatCommandResolver::Create([&](const std::shared_ptr<ChatCommandResolver>& resolver)
    {
      return chat_commands(task_manager, std::move(online_user_generator), std::move(user_searcher), resolver);
    });
  }

  template<class T, class K>
  static auto key_mapping(K&& k)
  {
    return std::make_pair(std::forward<K>(k), T::command_index);
  }

  std::unordered_map<std::string, std::string> create_key_mapping()
  {
    return {
      key_mapping<DisconnectChatCommand>("/exit"),
      key_mapping<RequestOnlineUserChatCommand>("/who"),
      key_mapping< RequestMessageChatCommand>("/w")
    };
  }
}
