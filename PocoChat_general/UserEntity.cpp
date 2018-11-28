#include "pch.h"

#include "NetworkCommunicator.h"
#include "UserEntity.h"


namespace PocoChatGeneral
{
  UserEntity::UserEntity(std::string name, const std::shared_ptr<NetworkCommunicator>& network_communicator) : name_(std::move(name)), network_communicator_(network_communicator)
  {
  }


  UserEntity::~UserEntity()
  {
  }

  const std::string& UserEntity::name() const
  {
    return name_;
  }

  std::shared_ptr<NetworkCommunicator> UserEntity::network_communicator()
  {
    return network_communicator_;
  }
}
