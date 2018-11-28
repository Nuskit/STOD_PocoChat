#ifndef CHAT_COMMAND_FACTORY_H
#define CHAT_COMMAND_FACTORY_H

#include <memory>
#include <unordered_map>
#include <string>
#include <functional>
#include <vector>


namespace PocoChatGeneral
{
  class ChatCommandResolver;
  class TaskManager;
  class UserEntity;

  std::shared_ptr<ChatCommandResolver> create_resolver(const std::shared_ptr<TaskManager>& task_manager=nullptr,
    std::function<std::vector<std::string>(int)> online_user_generator = [](int) -> std::vector<std::string> {return {}; },
    std::function<std::shared_ptr<UserEntity>(const std::string&)> user_searcher = [](const std::string&) {return std::shared_ptr<UserEntity>{nullptr}; });

  std::unordered_map<std::string, std::string> create_key_mapping();
}

#endif
