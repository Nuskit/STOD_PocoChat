#ifndef SERVER_USER_ENTITY_H
#define SERVER_USER_ENTITY_H

#include <string>
#include <memory>

#include "UserEntity.h"

namespace PocoChatGeneral
{
  class NetworkCommunicator;
}

class ServerUserEntity : public PocoChatGeneral::UserEntity
{
public:
  ServerUserEntity(std::string name, const std::shared_ptr<PocoChatGeneral::NetworkCommunicator>& network_communicator);

  void connected();
  void disconnected();

  bool online() const;
private:
  bool online_;
};

#endif
