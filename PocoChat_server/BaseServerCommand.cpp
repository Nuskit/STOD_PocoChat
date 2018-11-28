#include "pch.h"

#include "BaseServerCommand.h"


BaseServerCommand::BaseServerCommand(std::string command_name) : command_name_(std::move(command_name))
{
}

const std::string& BaseServerCommand::name() const
{
  return command_name_;
}

bool BaseServerCommand::handle(const std::string& args)
{
  return handleImpl(args);
}
