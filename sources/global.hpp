#pragma once

#include <vector>

class Client;

#include "utils/sockets.hpp"
#include "client.hpp"
#include "params.hpp"

typedef struct s_global
{
	t_params				params;
	t_socket				server;
	std::vector<Client>	clients;
}	t_global;