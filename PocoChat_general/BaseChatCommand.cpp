#include "pch.h"

#include <iterator>

#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/JSONException.h"

#include "Tools.h"
#include "BaseChatCommand.h"


namespace PocoChatGeneral
{
  BaseChatCommand::CommandNetworkData::CommandNetworkData(std::string network_index) : network_index_(std::move(network_index))
  {
  }

  std::string BaseChatCommand::CommandNetworkData::ExtractCommand(const std::string& message)
  {
    std::string command_name;
    try
    {
      command_name = PartialParse(message).extract<Poco::JSON::Object::Ptr>()->getValue<std::string>(command_field);
    }
    catch (const Poco::JSON::JSONException& e)
    {
      debug_message("Json exception: ", e.what());
      command_name = "";
    }
    return command_name;
  }

  void BaseChatCommand::CommandNetworkData::parseImpl(const Poco::Dynamic::Var& message)
  {
    parseData(message.extract<Poco::JSON::Object::Ptr>()->get(data_field));
  }

  void BaseChatCommand::CommandNetworkData::generateImpl(const std::string& data, std::ostream& out) const
  {
    Poco::JSON::Object object;
    object.set(command_field, network_index_);
    object.set(data_field, generateData(data));
    object.stringify(out);
  }

  BaseChatCommand::BaseChatCommand(std::string command_name, std::string network_index) :
    command_name_(std::move(command_name)), network_index_(std::move(network_index))
  {
  }

  const std::string& BaseChatCommand::name() const
  {
    return command_name_;
  }

  bool BaseChatCommand::handleClient(UserEntity& command_source, const std::string& args) const
  {
    return handleClientImpl(command_source, args);
  }

  bool BaseChatCommand::handleServer(UserEntity& command_source, const std::string& args) const
  {
    return handleServerImpl(command_source, args);
  }

  std::string BaseChatCommand::HandleClientData(const std::unique_ptr<CommandNetworkData>& network_data, const std::string & args)
  {
    std::stringstream buffer;
    network_data->generate(args, buffer);
    return buffer.str();
  }
}
