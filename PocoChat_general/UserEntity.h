#ifndef USER_ENTITY_H
#define USER_ENTITY_H

#include <string>
#include <memory>


namespace PocoChatGeneral
{
  class NetworkCommunicator;

  class UserEntity
  {
  public:
    UserEntity(std::string name, const std::shared_ptr<NetworkCommunicator>& network_communicator);
    UserEntity(const UserEntity&) = default;
    UserEntity(UserEntity&&) = default;
    UserEntity& operator=(const UserEntity&) = default;
    UserEntity& operator=(UserEntity&&) = default;
    virtual ~UserEntity();

    const std::string& name() const;

    std::shared_ptr<NetworkCommunicator> network_communicator();

  private:
    std::string name_;
    std::shared_ptr<NetworkCommunicator> network_communicator_;
  };
}

#endif
