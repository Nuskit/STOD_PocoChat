#ifndef RESPONSE_ONLINE_USER_CHAT_COMMAND_H
#define RESPONSE_ONLINE_USER_CHAT_COMMAND_H

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
  class UserEntity;

  class ResponseOnlineUserChatCommand : public BaseChatCommand
  {
  public:
    static constexpr char command_index[] = "response_online_user";
  protected:
    class ResponseOnlineUserCommandData : public BaseChatCommand::CommandNetworkData
    {
    public:
      ResponseOnlineUserCommandData(std::string network_name, std::function<std::vector<std::string>(int)> online_user_generator);

      const std::vector<std::string>& onlineUsers() const;
    private:
      static constexpr char online_field[] = "online";

      Poco::Dynamic::Var generateOnlineUsers(int max_number) const;

      virtual Poco::Dynamic::Var generateData(const std::string& data) const override;
      virtual void parseData(const Poco::Dynamic::Var& data) override;

      std::vector<std::string> online_users_;
      std::function<std::vector<std::string>(int)> online_user_generator_;
    };
  public:
    using NetworkDataInitializer = ResponseOnlineUserCommandData;

  public:
    ResponseOnlineUserChatCommand(std::function<std::vector<std::string>(int)> online_user_generator);
  private:
    virtual bool handleServerImpl(UserEntity & command_source, const std::string & args) const override;
    virtual bool handleClientImpl(UserEntity & command_source, const std::string & args) const override;

    std::function<std::vector<std::string>(int)> online_user_generator_;
  };
}

#endif
