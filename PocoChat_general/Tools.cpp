#include "pch.h"

#include "NetworkCommunicator.h"
#include "Tools.h"


namespace PocoChatGeneral
{
  void parse_input(const std::function<bool(const std::string&, const std::string&)>& resolver)
  {
    std::string command_line;
    size_t command_args_pos;
    do
    {
      std::getline(std::cin, command_line);
      command_args_pos = command_line.find(' ');

    } while (resolver(
      command_line.substr(0, command_args_pos),
      command_args_pos == std::string::npos ? std::string() : command_line.substr(command_args_pos + 1)));
  }

  void listen_input(NetworkCommunicator& network_communicator, const std::function<bool(const std::string&)>& resolver)
  {
    std::string message;
    do
    {
      message = network_communicator.getMessage();
    } while (!message.empty() && resolver(message));
  }
}
