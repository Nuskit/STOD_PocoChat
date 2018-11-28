#include "pch.h"

#include <assert.h> 

#include "Poco/Net/StreamSocket.h"
#include "Poco/JSON/Parser.h"
#include "Poco/FIFOBufferStream.h"

#include "NetworkCommunicator.h"


namespace PocoChatGeneral
{

  NetworkCommunicator::NetworkCommunicator(Poco::Net::StreamSocket& socket) : socket_(socket)
  {
  }

  void NetworkCommunicator::changeSocket(Poco::Net::StreamSocket& new_socket)
  {
    socket_ = new_socket;
  }

  void NetworkCommunicator::sendMessage(const std::string& message)
  {
    auto send_bytes = socket_.sendBytes(message.data(), static_cast<int>(message.size()));
    assert(send_bytes == message.size());
  }

  std::string NetworkCommunicator::getMessage()
  {
    Poco::FIFOBuffer received_buffer(4096);
    auto received_number = socket_.receiveBytes(received_buffer);
    return { received_buffer.begin(), static_cast<size_t>(received_number) };
  }
}
