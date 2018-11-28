#ifndef DEFINITION_H
#define DEFINITION_H

#include <string>

namespace PocoChatGeneral
{
  struct ServerInformation
  {
    std::string ip;
    int port;
  };

  static const ServerInformation kServerInformation = 
  {
    "127.0.0.1",
    50500
  };
}

#endif
