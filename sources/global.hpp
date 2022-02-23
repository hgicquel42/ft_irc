#pragma once

#include <vector>
#include <map>

class Client;
class Channel;

#include "utils/sockets.hpp"

#include "numerics.hpp"
#include "channel.hpp"
#include "client.hpp"
#include "params.hpp"

using namespace std;

typedef vector<Client*>			VClients;
typedef vector<Channel*>		VChannels;
typedef map<string, Channel*>	MChannels;

typedef struct s_global
{
	t_params	params;
	t_socket	server;
	VClients	clients;
	MChannels	channels;
}	t_global;