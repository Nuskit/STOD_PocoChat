#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <iostream>
#include <utility>
#include <functional>

#include "Poco/Net/SocketAddress.h"

#include "Definition.h"


namespace PocoChatGeneral
{
  class NetworkCommunicator;

  template<class First, class ...Last>
  inline void _message_impl(std::ostream& stream, First&& first, Last&&... last)
  {
    stream << std::forward<First>(first);
    if constexpr (sizeof...(Last) > 0)
    {
      _message_impl(stream, std::forward<Last>(last)...);
    }
    else
    {
      stream << std::endl;
    }
  }

  template <typename T>
  std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    if (!v.empty()) {
      out << '[';
      std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
      out << "\b\b]";
    }
    return out;
  }

  template<class ...Args>
  void debug_message(Args&&... args)
  {
    _message_impl(std::cout, "[Debug]: ", std::forward<Args>(args)...);
  }

  template<class ...Args>
  void info_message(Args&&... args)
  {
    _message_impl(std::cout, "[Info]: ", std::forward<Args>(args)...);
  }

  template<class S>
  S create_socket(const PocoChatGeneral::ServerInformation& kServerInformation)
  {
    Poco::Net::SocketAddress address(kServerInformation.ip, kServerInformation.port);
    return S{ address };
  }

  void parse_input(const std::function<bool(const std::string&, const std::string&)>& resolver);
  void listen_input(NetworkCommunicator& network_communicator, const std::function<bool(const std::string&)>& resolver);
}

#endif
