#include "pch.h"

#include <algorithm>
#include <exception>

#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Object.h"

#include "Tools.h"
#include "UserEntity.h"
#include "NetworkCommunicator.h"
#include "ResponseOnlineUserChatCommand.h"


namespace PocoChatGeneral
{
  ResponseOnlineUserChatCommand::ResponseOnlineUserCommandData::ResponseOnlineUserCommandData(std::string network_name, 
    std::function<std::vector<std::string>(int)> online_user_generator) :
    BaseChatCommand::CommandNetworkData(std::move(network_name)), online_users_(), online_user_generator_(std::move(online_user_generator))
  {
  }

  Poco::Dynamic::Var ResponseOnlineUserChatCommand::ResponseOnlineUserCommandData::generateOnlineUsers(int max_number) const
  {
    auto online_users = online_user_generator_(max_number);

    Poco::Dynamic::Array online_user_array;
    online_user_array.reserve(std::size(online_users));

    std::copy(online_users.begin(), online_users.end(),
      std::insert_iterator<decltype(online_user_array)>(online_user_array, online_user_array.begin()));

    return online_user_array;
  }

  Poco::Dynamic::Var ResponseOnlineUserChatCommand::ResponseOnlineUserCommandData::generateData(const std::string& data) const
  {
    Poco::JSON::Object object;
    int max_number;
    try
    {
      max_number = std::stoi(data);
    }
    catch (const std::exception&)
    {
      max_number = 0;
    }

    object.set(online_field, generateOnlineUsers(max_number));
    return object;
  }

  const std::vector<std::string>& ResponseOnlineUserChatCommand::ResponseOnlineUserCommandData::onlineUsers() const
  {
    return online_users_;
  }

  void ResponseOnlineUserChatCommand::ResponseOnlineUserCommandData::parseData(const Poco::Dynamic::Var& data)
  {
    auto user_array = data.extract<Poco::JSON::Object::Ptr>()->getArray(online_field);
    online_users_.reserve(std::size(*user_array));
    std::transform(user_array->begin(), user_array->end(), std::insert_iterator<decltype(online_users_)>(online_users_, online_users_.begin()),
      [](auto& element)
    {
      return element.toString();
    });
  }

  ResponseOnlineUserChatCommand::ResponseOnlineUserChatCommand(std::function<std::vector<std::string>(int)> online_user_generator) : 
    BaseChatCommand("ResponseOnlineUserCommand", command_index), online_user_generator_(std::move(online_user_generator))
  {
  }

  bool ResponseOnlineUserChatCommand::handleServerImpl(UserEntity & command_source, const std::string & args) const
  {
    command_source.network_communicator()->sendMessage(HandleClientData(network_data<ResponseOnlineUserChatCommand>(online_user_generator_), args));
    return true;
  }

  bool ResponseOnlineUserChatCommand::handleClientImpl(UserEntity & command_source, const std::string & args) const
  {
    auto message_data = network_data<ResponseOnlineUserChatCommand>(online_user_generator_);
    message_data->parse(args);
    info_message("Online users: ", message_data->onlineUsers());
    return true;
  }
}
