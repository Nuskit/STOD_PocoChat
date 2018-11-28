#ifndef NETWORK_COMMUNICATOR_H
#define NETWORK_COMMUNICATOR_H

#include <string>


namespace Poco
{
  namespace Net
  {
    class StreamSocket;
  }
}

namespace PocoChatGeneral
{
  class NetworkCommunicator
  {
  public:
    explicit NetworkCommunicator(Poco::Net::StreamSocket& socket);

    void changeSocket(Poco::Net::StreamSocket& new_socket);

    void sendMessage(const std::string& message);
    std::string getMessage();
  private:
    Poco::Net::StreamSocket& socket_;
  };
}

#endif
