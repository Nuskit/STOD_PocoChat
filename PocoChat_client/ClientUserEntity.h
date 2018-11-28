#ifndef CLIENT_USER_ENTITY_H
#define CLIENT_USER_ENTITY_H

#include "UserEntity.h"


namespace Poco
{
  namespace Net
  {
    class StreamSocket;
  }
}

class ClientUserEntity : public PocoChatGeneral::UserEntity
{
public:
  ClientUserEntity(Poco::Net::StreamSocket& socket);
  virtual ~ClientUserEntity();
};

#endif
