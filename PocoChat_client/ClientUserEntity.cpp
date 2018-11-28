#include "pch.h"

#include <memory>

#include "NetworkCommunicator.h"

#include "ClientUserEntity.h"


ClientUserEntity::ClientUserEntity(Poco::Net::StreamSocket& socket) : UserEntity("current user", std::make_shared<PocoChatGeneral::NetworkCommunicator>(socket))
{
}


ClientUserEntity::~ClientUserEntity()
{
}
