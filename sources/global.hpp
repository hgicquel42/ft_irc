#pragma once

#include <vector>

#include "utils/sockets.hpp"
#include "params.hpp"

typedef struct s_global
{
	t_params				params;
	t_socket				server;
	std::vector<t_socket>	clients;
}	t_global;