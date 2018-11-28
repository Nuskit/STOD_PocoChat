#include "pch.h"

#include "ExitServerCommand.h"


ExitServerCommand::ExitServerCommand() : BaseServerCommand("ExitCommand")
{
}

bool ExitServerCommand::handleImpl(const std::string & args)
{
  return false;
}
