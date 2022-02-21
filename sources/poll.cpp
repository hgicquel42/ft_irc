#include "poll.hpp"

#include <iostream>
#include <unistd.h>

void	ft_accept(t_global& global)
{
	t_socket	client;

	std::cout << "New client connected\n";

	client = ft_saccept(global.server);
	global.clients.push_back(client);

	// TODO: envoyer le MOTD
	(void)global;
}

bool	ft_read(t_global& global, t_socket& client)
{
	char							buffer[1024];
	int								nread;

	nread = read(client.socket, &buffer, 1024);
	if (nread == 0)
	{
		std::cout << "Client disconnected\n";
		close(client.socket);
		return (false);
	}
	else
	{
		buffer[nread] = 0;
		std::cout << buffer;
		(void) global;
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
	std::vector<t_socket>::iterator	iclient;

	while (true)
	{
		FD_ZERO(&pollset);
		FD_SET(global.server.socket, &pollset);
		for (iclient = global.clients.begin(); iclient != global.clients.end(); iclient++)
			FD_SET(iclient->socket, &pollset);
		if (select(FD_SETSIZE, &pollset, NULL, NULL, NULL) == -1)
			throw Exception(strerror(errno));
		if (FD_ISSET(global.server.socket, &pollset))
		{
			ft_accept(global);
			continue ;
		}
		for (iclient = global.clients.begin(); iclient != global.clients.end(); iclient++)
			if (FD_ISSET(iclient->socket, &pollset))
				break ;
		if (iclient == global.clients.end())
			continue ;
		if (!ft_read(global, *iclient))
			global.clients.erase(iclient);
	}
}
