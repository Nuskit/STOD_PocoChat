#include "pch.h"

#include <memory>
#include <vector>
#include <string>

#include "Poco/Net/NetException.h"

#include "ChatCommandFactory.h"
#include "Definition.h"
#include "ChatCommandResolver.h"
#include "Tools.h"

#include "ClientCommandResolver.h"
#include "ClientUserEntity.h"

#include "ChatClient.h"


std::unique_ptr<ChatClient> DefaultClient()
{
  return std::make_unique<ChatClient>(PocoChatGeneral::kServerInformation,
    std::make_unique<ClientCommandResolver>(PocoChatGeneral::create_key_mapping(),
      PocoChatGeneral::create_resolver()));
}


int main()
{
  try
  {
    DefaultClient()->run();
  } 
  catch (const Poco::Net::NetException& e)
  {
    PocoChatGeneral::debug_message("Socket exception: ", e.what());
  }
}
