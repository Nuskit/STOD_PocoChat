#ifndef DISCONNECT_CHAT_COMMAND_H
#define DISCONNECT_CHAT_COMMAND_H

#include <string>

#include "BaseChatCommand.h"


namespace Poco
{
  namespace Dynamic
  {
    class Var;
  }
}

class UserEntity;

namespace PocoChatGeneral
{
  class DisconnectChatCommand : public BaseChatCommand
  {
  public:
    static constexpr char command_index[] = "disconnect";
  protected:
    class DisconnectCommandData : public BaseChatCommand::CommandNetworkData
    {
    public:
      explicit DisconnectCommandData(std::string network_name);

      const std::string& reason() const;
    private:
      static constexpr char reason_field[] = "reason";

      virtual Poco::Dynamic::Var generateData(const std::string& data) const override;
      virtual void parseData(const Poco::Dynamic::Var& data) override;

      std::string reason_;
    };
  public:
    using NetworkDataInitializer = DisconnectCommandData;

  public:
    DisconnectChatCommand();
  private:
    virtual bool handleServerImpl(UserEntity & command_source, const std::string & args) const override;
    virtual bool handleClientImpl(UserEntity & command_source, const std::string & args) const override;
  };
}

#endif
