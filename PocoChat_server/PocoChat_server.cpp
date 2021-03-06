#include "pch.h"

#include <memory>

#include "Poco/Net/NetException.h"

#include "UserEntity.h"
#include "ChatCommandFactory.h"
#include "ChatCommandResolver.h"
#include "TaskManager.h"
#include "Tools.h"

#include "UserEntities.h"
#include "ServerUserEntity.h"
#include "ServerCommandFactory.h"
#include "ServerCommandResolver.h"
#include "TcpServer.h"


std::unique_ptr<TcpServer> DefaultServer()
{
  // TODO: hide user entity using
  auto user_entities = std::make_shared<UserEntities>();

  return std::make_unique<TcpServer>(
    user_entities,
    PocoChatGeneral::kServerInformation,
    PocoChatGeneral::create_resolver(PocoChatGeneral::TaskManager::Create(),
      [user_entities](int max_number)
      {
        return user_entities->onlineUsers(max_number);
      },
      [user_entities](const std::string& searched_user)
      {
        return std::dynamic_pointer_cast<PocoChatGeneral::UserEntity>(user_entities->searchUser(searched_user));
      }),
    create_server_resolver());
}


int main()
{
  try
  {
    DefaultServer()->run();
  }
  catch (const Poco::Net::NetException& e)
  {
    PocoChatGeneral::debug_message("Socket exception: ", e.what());
  }
}
