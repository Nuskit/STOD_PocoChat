#include "pch.h"

#include <algorithm>
#include <exception>

#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Array.h"
#include "Poco/JSON/Object.h"

#include "Tools.h"
#include "UserEntity.h"
#include "NetworkCommunicator.h"
#include "ResponseMessageChatCommand.h"


namespace PocoChatGeneral
{
  ResponseMessageChatCommand::ResponseMessageCommandData::ResponseMessageCommandData(std::string network_name) :
    BaseChatCommand::CommandNetworkData(std::move(network_name)), source_user_(), message_()
  {
  }

  Poco::Dynamic::Var ResponseMessageChatCommand::ResponseMessageCommandData::generateData(const std::string& data) const
  {
    Poco::JSON::Object object;
    auto message_pos = data.find(' ');
    std::string source_user = data.substr(0, message_pos);
    std::string message = message_pos == std::string::npos ? std::string() : data.substr(message_pos + 1);

    object.set(source_field, source_user);
    object.set(message_field, message);
    return object;
  }

  const std::string& ResponseMessageChatCommand::ResponseMessageCommandData::sourceUser() const
  {
    return source_user_;
  }

  const std::string& ResponseMessageChatCommand::ResponseMessageCommandData::message() const
  {
    return message_;
  }

  void ResponseMessageChatCommand::ResponseMessageCommandData::parseData(const Poco::Dynamic::Var& data)
  {
    auto data_object = data.extract<Poco::JSON::Object::Ptr>();

    source_user_ = data_object->getValue<decltype(source_user_)>(source_field);
    message_ = data_object->getValue<decltype(message_)>(message_field);
  }

  ResponseMessageChatCommand::ResponseMessageChatCommand() : BaseChatCommand("ResponseMessageCommand", command_index)
  {
  }

  bool ResponseMessageChatCommand::handleServerImpl(UserEntity & command_source, const std::string & args) const
  {
    command_source.network_communicator()->sendMessage(HandleClientData(network_data<ResponseMessageChatCommand>(), args));
    return true;
  }

  bool ResponseMessageChatCommand::handleClientImpl(UserEntity & command_source, const std::string & args) const
  {
    auto message_data = network_data<ResponseMessageChatCommand>();
    message_data->parse(args);
    info_message("Message [", message_data->sourceUser(), "]: ", message_data->message());
    return true;
  }
}
