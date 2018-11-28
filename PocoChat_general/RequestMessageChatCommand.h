#ifndef REQUEST_MESSAGE_CHAT_COMMAND_H
#define REQUEST_MESSAGE_CHAT_COMMAND_H

#include <string>
#include <memory>
#include <functional>

#include "BaseChatCommand.h"


namespace Poco
{
  namespace Dynamic
  {
    class Var;
  }
}

namespace PocoChatGeneral
{
  class TaskManager;

  class RequestMessageChatCommand : public BaseChatCommand
  {
  public:
    static constexpr char command_index[] = "request_message";
  protected:
    class RequestMessageCommandData : public BaseChatCommand::CommandNetworkData
    {
    public:
      explicit RequestMessageCommandData(std::string network_name);

      const std::string& destination() const;
      const std::string& message() const;
    private:
      static constexpr char destination_field[] = "destination";
      static constexpr char message_field[] = "message";

      virtual Poco::Dynamic::Var generateData(const std::string& data) const override;
      virtual void parseData(const Poco::Dynamic::Var& data) override;

      std::string destination_;
      std::string message_;
    };
  public:
    using NetworkDataInitializer = RequestMessageCommandData;

  public:
    RequestMessageChatCommand(const std::shared_ptr<TaskManager>& task_manager, NextCommand send_user_command,
      std::function<std::shared_ptr<UserEntity>(const std::string&)> user_searcher);
  private:
    virtual bool handleServerImpl(UserEntity & command_source, const std::string & args) const override;
    virtual bool handleClientImpl(UserEntity & command_source, const std::string & args) const override;

    std::shared_ptr<TaskManager> task_manager_;
    NextCommand send_user_command_;
    std::function<std::shared_ptr<UserEntity>(const std::string&)> user_searcher_;
  };
}

#endif
