#ifndef BASE_CHAT_COMMAND_H
#define BASE_CHAT_COMMAND_H

#include <string>
#include <memory>
#include <utility>

#include "NetworkData.h"


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

  class BaseChatCommand
  {
  public:
    class CommandNetworkData : public NetworkData
    {
    public:
      explicit CommandNetworkData(std::string network_index);

      static std::string ExtractCommand(const std::string& message);
    private:
      static constexpr char command_field[] = "command";
      static constexpr char data_field[] = "data";

      virtual void parseImpl(const Poco::Dynamic::Var& message) override;
      virtual void generateImpl(const std::string& data, std::ostream& out) const override;
      virtual Poco::Dynamic::Var generateData(const std::string& data) const = 0;
      virtual void parseData(const Poco::Dynamic::Var& data) = 0;

      std::string network_index_;
    };
  public:
    using NetworkDataInitializer = CommandNetworkData;
    using NextCommand = std::function<void(UserEntity&, const std::string&)>;

  public:
    BaseChatCommand(std::string command_name, std::string network_index);
    BaseChatCommand(const BaseChatCommand&) = default;
    BaseChatCommand(BaseChatCommand&&) = default;
    BaseChatCommand& operator=(const BaseChatCommand&) = default;
    BaseChatCommand& operator=(BaseChatCommand&&) = default;
    virtual ~BaseChatCommand() = default;

    bool handleServer(UserEntity& command_source, const std::string& args) const;
    bool handleClient(UserEntity& command_source, const std::string& args) const;
    const std::string& name() const;

  protected:
    template<class N, class ... Args>
    auto network_data(Args&&...args) const
    {
      return std::make_unique<N::NetworkDataInitializer>(network_index_, std::forward<Args>(args)...);
    }

    static std::string HandleClientData(const std::unique_ptr<CommandNetworkData>& network_data, const std::string & args);

  private:
    virtual bool handleServerImpl(UserEntity& command_source, const std::string& args) const = 0;
    virtual bool handleClientImpl(UserEntity& command_source, const std::string& args) const = 0;

    std::string command_name_;
    std::string network_index_;
  };
}

#endif
