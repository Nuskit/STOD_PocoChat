#ifndef BASE_SERVER_COMMAND_H
#define BASE_SERVER_COMMAND_H

#include <string>


class BaseServerCommand
{
public:
  explicit BaseServerCommand(std::string command_name);
  BaseServerCommand(const BaseServerCommand&) = default;
  BaseServerCommand(BaseServerCommand&&) = default;
  BaseServerCommand& operator=(const BaseServerCommand&) = default;
  BaseServerCommand& operator=(BaseServerCommand&&) = default;
  virtual ~BaseServerCommand() = default;

  bool handle(const std::string& args);
  const std::string& name() const;
private:
  virtual bool handleImpl(const std::string& args) = 0;

  std::string command_name_;
};


#endif
