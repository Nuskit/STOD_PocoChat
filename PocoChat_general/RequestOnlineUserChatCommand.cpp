#include "pch.h"

#include <algorithm>
#include <exception>

#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Object.h"
#include "Poco/Runnable.h"

#include "Tools.h"
#include "UserEntity.h"
#include "NetworkCommunicator.h"
#include "TaskManager.h"
#include "RequestOnlineUserChatCommand.h"


namespace PocoChatGeneral
{
  RequestOnlineUserChatCommand::RequestOnlineUserCommandData::RequestOnlineUserCommandData(std::string network_name) :
    BaseChatCommand::CommandNetworkData(std::move(network_name)), max_number_(0)
  {
  }

  Poco::Dynamic::Var RequestOnlineUserChatCommand::RequestOnlineUserCommandData::generateData(const std::string& data) const
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
    object.set(max_number_field, std::max(0, max_number));
    return object;
  }

  int RequestOnlineUserChatCommand::RequestOnlineUserCommandData::maxNumber() const
  {
    return max_number_;
  }

  void RequestOnlineUserChatCommand::RequestOnlineUserCommandData::parseData(const Poco::Dynamic::Var& data)
  {
    max_number_ = data.extract<Poco::JSON::Object::Ptr>()->getValue<decltype(max_number_)>(max_number_field);
  }

  RequestOnlineUserChatCommand::RequestOnlineUserChatCommand(const std::shared_ptr<TaskManager>& task_manager, NextCommand next_command) :
    BaseChatCommand("RequestOnlineUserCommand", command_index), task_manager_(task_manager), next_command_(std::move(next_command))
  {
  }

  class ResponseOnlineRunnable : public Poco::Runnable
  {
  public :
    ResponseOnlineRunnable(UserEntity& command_source, RequestOnlineUserChatCommand::NextCommand next_command, int max_number) :
      Poco::Runnable(), next_command_(std::move(next_command)), command_source_(command_source), max_number_(max_number)
    {
    }

    virtual void run() override
    {
      next_command_(command_source_, std::to_string(max_number_));
    }
  private:
    std::function<void(UserEntity&, const std::string&)> next_command_;
    UserEntity& command_source_;
    int max_number_;
  };

  bool RequestOnlineUserChatCommand::handleServerImpl(UserEntity & command_source, const std::string & args) const
  {
    auto message_data = network_data<RequestOnlineUserChatCommand>();
    message_data->parse(args);
    info_message("Request for online user with max number = ", message_data->maxNumber());
    task_manager_->add_task(std::make_unique<ResponseOnlineRunnable>(command_source, next_command_, message_data->maxNumber()));
    return true;
  }

  bool RequestOnlineUserChatCommand::handleClientImpl(UserEntity & command_source, const std::string & args) const
  {
    command_source.network_communicator()->sendMessage(HandleClientData(network_data<RequestOnlineUserChatCommand>(), args));
    return true;
  }
}
