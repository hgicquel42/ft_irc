#include "poll.hpp"

#include <iostream>
#include <unistd.h>

#include "global.hpp"
#include "client.hpp"

void	ft_accept(t_global& global)
{
	t_socket	socket;

	socket = ft_saccept(global.server);
	Client client(global, socket);
	global.clients.push_back(client);

	client.onConnect();
}

bool	ft_read(Client& client, t_global& global)
{
	char		buffer[1024];
	int			nread;
	std::string	packet;

	nread = read(client.socket.file, &buffer, 1024);
	if (nread == 0)
	{
		client.onDisconnect();
		return (false);
	}
	else
	{
		buffer[nread] = 0;
		packet = std::string(buffer);
		client.onPacket(packet.substr(0, packet.length() - 2), global);
		return (true);
	}
}

/**
 * @brief polling loop
 * 
 * @param global 
 */
void	ft_poll(t_global& global)
{
	fd_set							pollset;
	std::vector<Client>::iterator	iclient;

	while (true)
	{
		FD_ZERO(&pollset);
		FD_SET(global.server.file, &pollset);
		for (iclient = global.clients.begin(); iclient != global.clients.end(); iclient++)
			FD_SET(iclient->socket.file, &pollset);
		if (select(FD_SETSIZE, &pollset, NULL, NULL, NULL) == -1)
			throw Exception(strerror(errno));
		if (FD_ISSET(global.server.file, &pollset))
		{
			ft_accept(global);
			continue ;
		}
		for (iclient = global.clients.begin(); iclient != global.clients.end(); iclient++)
			if (FD_ISSET(iclient->socket.file, &pollset))
				break ;
		if (iclient == global.clients.end())
			continue ;
		if (!ft_read(*iclient, global))
			global.clients.erase(iclient);
	}
}
