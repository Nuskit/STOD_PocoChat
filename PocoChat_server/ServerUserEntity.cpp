#include "pch.h"

#include "Tools.h"

#include "ServerUserEntity.h"



ServerUserEntity::ServerUserEntity(std::string name, const std::shared_ptr<PocoChatGeneral::NetworkCommunicator>& network_communicator) : 
  PocoChatGeneral::UserEntity(std::move(name), network_communicator), online_(false)
{
}

void ServerUserEntity::connected()
{
  online_ = true;
  PocoChatGeneral::info_message("Client ", name(), " is online");
}

void ServerUserEntity::disconnected()
{
  online_ = false;
  PocoChatGeneral::info_message("Client ", name(), " is offline");
}

bool ServerUserEntity::online() const
{
  return online_;
}
