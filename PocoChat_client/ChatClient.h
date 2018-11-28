#ifndef CHAT_CLIENT_H
#define CHAT_CLIENT_H

#include <memory>
#include <atomic>
#include <thread>

#include "Poco/Net/StreamSocket.h"


namespace PocoChatGeneral
{
  struct ServerInformation;
}

class ClientCommandResolver;
class ClientUserEntity;

class ChatClient
{
public:
  ChatClient(const PocoChatGeneral::ServerInformation& kServerInformation, std::unique_ptr<ClientCommandResolver>&& command_resolver);
  ChatClient(const ChatClient&) = delete;
  ChatClient(ChatClient&&) = delete;
  ChatClient& operator=(const ChatClient&) = delete;
  ChatClient& operator=(ChatClient&&) = delete;
  void run();
  ~ChatClient();

private:
  void received_async();

  Poco::Net::StreamSocket socket_;
  std::unique_ptr<ClientUserEntity> user_entity_;
  std::unique_ptr<ClientCommandResolver> command_resolver_;
  std::atomic_bool is_working_;
  std::thread async_listener_;
};

#endif
