#ifndef USER_ENTITIES_H
#define USER_ENTITIES_H

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <mutex>


namespace Poco
{
  namespace Net
  {
    class StreamSocket;
  }
}

class ServerUserEntity;

class UserEntities
{
public:
  std::shared_ptr<ServerUserEntity> getUser(const std::string& user_identifier) const;
  bool existUser(const std::string& user_identifier) const;
  void registerUser(const std::string user_identifier, std::unique_ptr<ServerUserEntity>&& user_entity);

  static std::unique_ptr<ServerUserEntity> CreateUser(const Poco::Net::StreamSocket& socket);

  std::vector<std::string> onlineUsers(int max_number) const;
  std::shared_ptr<ServerUserEntity> searchUser(const std::string& user_identifier) const;
private:
  std::unordered_map<std::string, std::shared_ptr<ServerUserEntity>>::const_iterator userFind(const std::string& user_identifier) const;

  std::unordered_map<std::string, std::shared_ptr<ServerUserEntity>> user_entities_;
  mutable std::mutex user_mutex_;
};

#endif
