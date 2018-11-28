#include "pch.h"

#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Object.h"

#include "Tools.h"
#include "UserEntity.h"
#include "NetworkCommunicator.h"
#include "DisconnectChatCommand.h"


namespace PocoChatGeneral
{
  DisconnectChatCommand::DisconnectCommandData::DisconnectCommandData(std::string network_name) :
    BaseChatCommand::CommandNetworkData(std::move(network_name)), reason_()
  {
  }

  Poco::Dynamic::Var DisconnectChatCommand::DisconnectCommandData::generateData(const std::string& data) const
  {
    Poco::JSON::Object object;
    object.set(reason_field, data);
    return object;
  }

  const std::string& DisconnectChatCommand::DisconnectCommandData::reason() const
  {
    return reason_;
  }

  void DisconnectChatCommand::DisconnectCommandData::parseData(const Poco::Dynamic::Var& data)
  {
    reason_ = data.extract<Poco::JSON::Object::Ptr>()->getValue<decltype(reason_)>(reason_field);
  }

  DisconnectChatCommand::DisconnectChatCommand() : BaseChatCommand("DisconnectCommand", command_index)
  {
  }

  bool DisconnectChatCommand::handleServerImpl(UserEntity & command_source, const std::string & args) const
  {
    auto message_data = network_data<DisconnectChatCommand>();
    message_data->parse(args);
    info_message("Disconnecting with reason: ", message_data->reason());
    return false;
  }

  bool DisconnectChatCommand::handleClientImpl(UserEntity & command_source, const std::string & args) const
  {
    command_source.network_communicator()->sendMessage(HandleClientData(network_data<DisconnectChatCommand>(), args));
    return false;
  }
}
