#ifndef REQUEST_ONLINE_USER_CHAT_COMMAND_H
#define REQUEST_ONLINE_USER_CHAT_COMMAND_H

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

  class RequestOnlineUserChatCommand : public BaseChatCommand
  {
  public:
    static constexpr char command_index[] = "request_online_user";
  protected:
    class RequestOnlineUserCommandData : public BaseChatCommand::CommandNetworkData
    {
    public:
      explicit RequestOnlineUserCommandData(std::string network_name);

      int maxNumber() const;
    private:
      static constexpr char max_number_field[] = "limit";

      virtual Poco::Dynamic::Var generateData(const std::string& data) const override;
      virtual void parseData(const Poco::Dynamic::Var& data) override;

      int max_number_;
    };
  public:
    using NetworkDataInitializer = RequestOnlineUserCommandData;

  public:
    RequestOnlineUserChatCommand(const std::shared_ptr<TaskManager>& task_manager, NextCommand next_command);
  private:
    virtual bool handleServerImpl(UserEntity & command_source, const std::string & args) const override;
    virtual bool handleClientImpl(UserEntity & command_source, const std::string & args) const override;

    std::shared_ptr<TaskManager> task_manager_;
    NextCommand next_command_;
  };
}

#endif
