#ifndef RESPONSE_MESSAGE_CHAT_COMMAND_H
#define RESPONSE_MESSAGE_CHAT_COMMAND_H

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

  class ResponseMessageChatCommand : public BaseChatCommand
  {
  public:
    static constexpr char command_index[] = "response_message";
  protected:
    class ResponseMessageCommandData : public BaseChatCommand::CommandNetworkData
    {
    public:
      ResponseMessageCommandData(std::string network_name);

      const std::string& sourceUser() const;
      const std::string& message() const;
    private:
      static constexpr char source_field[] = "source";
      static constexpr char message_field[] = "message";

      virtual Poco::Dynamic::Var generateData(const std::string& data) const override;
      virtual void parseData(const Poco::Dynamic::Var& data) override;

      std::string source_user_;
      std::string message_;
    };

  public:
    using NetworkDataInitializer = ResponseMessageCommandData;

  public:
    ResponseMessageChatCommand();
  private:
    virtual bool handleServerImpl(UserEntity & command_source, const std::string & args) const override;
    virtual bool handleClientImpl(UserEntity & command_source, const std::string & args) const override;
  };
}

#endif
