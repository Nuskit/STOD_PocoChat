#include "pch.h"

#include <assert.h>

#include "Poco/Net/StreamSocket.h"

#include "NetworkCommunicator.h"

#include "ServerUserEntity.h"
#include "UserEntities.h"


std::shared_ptr<ServerUserEntity> UserEntities::getUser(const std::string& user_identifier) const
{
  assert(existUser(user_identifier));

  auto found_user = userFind(user_identifier);

  return found_user->second;
}

bool UserEntities::existUser(const std::string& user_identifier) const
{
  return userFind(user_identifier) != user_entities_.end();
}

void UserEntities::registerUser(const std::string user_identifier, std::unique_ptr<ServerUserEntity>&& user_entity)
{
  assert(!existUser(user_identifier));

  std::lock_guard<decltype(user_mutex_)> lock(user_mutex_);

  user_entities_[user_identifier] = std::move(user_entity);
}

std::unordered_map<std::string, std::shared_ptr<ServerUserEntity>>::const_iterator UserEntities::userFind(const std::string& user_identifier) const
{
  return user_entities_.find(user_identifier);
}

std::shared_ptr<ServerUserEntity> UserEntities::searchUser(const std::string& user_identifier) const
{
  std::shared_ptr<ServerUserEntity> found_user;
  if (existUser(user_identifier))
  {
    found_user = getUser(user_identifier);
    found_user = found_user->online() ? found_user : std::shared_ptr<ServerUserEntity>{ nullptr };
  }
  return found_user;
}

std::unique_ptr<ServerUserEntity> UserEntities::CreateUser(const Poco::Net::StreamSocket& socket)
{
  return std::make_unique<ServerUserEntity>(socket.peerAddress().toString(),
    std::make_unique<PocoChatGeneral::NetworkCommunicator>(const_cast<Poco::Net::StreamSocket&>(socket)));
}

std::vector<std::string> UserEntities::onlineUsers(int max_number) const
{
  std::vector<std::string> online_users;
  online_users.reserve(max_number);

  std::lock_guard<decltype(user_mutex_)> lock(user_mutex_);

  size_t checked_number = max_number == 0 ? std::size(user_entities_) : static_cast<size_t>(max_number);

  for (auto&& user_entity : user_entities_)
  {
    if (user_entity.second->online())
    {
      online_users.emplace_back(std::forward<decltype(user_entity)>(user_entity).first);
    }

    if (std::size(online_users) == checked_number)
    {
      break;
    }
  }

  online_users.shrink_to_fit();

  return online_users;
}
