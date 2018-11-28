#include "pch.h"

#include <algorithm>
#include <exception>
#include <sstream>

#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Object.h"
#include "Poco/Runnable.h"

#include "Tools.h"
#include "UserEntity.h"
#include "NetworkCommunicator.h"
#include "TaskManager.h"
#include "RequestMessageChatCommand.h"


namespace PocoChatGeneral
{
  RequestMessageChatCommand::RequestMessageCommandData::RequestMessageCommandData(std::string network_name) :
    BaseChatCommand::CommandNetworkData(std::move(network_name)), destination_(), message_()
  {
  }

  Poco::Dynamic::Var RequestMessageChatCommand::RequestMessageCommandData::generateData(const std::string& data) const
  {
    Poco::JSON::Object object;
    auto message_pos = data.find(' ');
    std::string destination = data.substr(0, message_pos);
    std::string message = message_pos == std::string::npos ? std::string() : data.substr(message_pos + 1);

    object.set(destination_field, destination);
    object.set(message_field, message);
    return object;
  }

  const std::string& RequestMessageChatCommand::RequestMessageCommandData::destination() const
  {
    return destination_;
  }

  const std::string& RequestMessageChatCommand::RequestMessageCommandData::message() const
  {
    return message_;
  }

  void RequestMessageChatCommand::RequestMessageCommandData::parseData(const Poco::Dynamic::Var& data)
  {
    auto data_object = data.extract<Poco::JSON::Object::Ptr>();

    destination_ = data_object->getValue<decltype(destination_)>(destination_field);
    message_ = data_object->getValue<decltype(message_)>(message_field);
  }

  RequestMessageChatCommand::RequestMessageChatCommand(const std::shared_ptr<TaskManager>& task_manager, NextCommand next_command,
    std::function<std::shared_ptr<UserEntity>(const std::string&)> user_searcher) :
    BaseChatCommand("RequestMessageCommand", command_index), task_manager_(task_manager), send_user_command_(std::move(next_command)), user_searcher_(std::move(user_searcher))
  {
  }

  class SendUserMessageRunnable : public Poco::Runnable
  {
  public:
    SendUserMessageRunnable(UserEntity& send_user_command_source, RequestMessageChatCommand::NextCommand next_command,
      std::string source, std::string message) :
      Poco::Runnable(), send_user_command_(std::move(next_command)), send_user_command_source_(send_user_command_source),
      source_(std::move(source)), message_(std::move(message))
    {
    }

    virtual void run() override
    {
      std::stringstream command_stream;
      command_stream  << source_ << " " << message_;
      send_user_command_(send_user_command_source_, command_stream.str());
    }
  private:
    RequestMessageChatCommand::NextCommand send_user_command_;
    UserEntity& send_user_command_source_;
    std::string source_;
    std::string message_;
  };

  bool RequestMessageChatCommand::handleServerImpl(UserEntity & command_source, const std::string & args) const
  {
    auto message_data = network_data<RequestMessageChatCommand>();
    message_data->parse(args);
    info_message("Request for sending message from `", command_source.name(), "' to '",
      message_data->destination(), "' with message: ", message_data->message());

    auto destination_user = user_searcher_(message_data->destination());
    if (destination_user)
    {
      task_manager_->add_task(std::make_unique<SendUserMessageRunnable>(*destination_user, send_user_command_, command_source.name(), message_data->message()));
    }
    else
    {
      info_message("Unknown destination: ", message_data->destination());
    }
    return true;
  }

  bool RequestMessageChatCommand::handleClientImpl(UserEntity & command_source, const std::string & args) const
  {
    command_source.network_communicator()->sendMessage(HandleClientData(network_data<RequestMessageChatCommand>(), args));
    return true;
  }
}
