#ifndef SERVER_COMMAND_FACTORY_H
#define SERVER_COMMAND_FACTORY_H

#include <memory>


class ServerCommandResolver;

std::unique_ptr<ServerCommandResolver> create_server_resolver();

#endif
