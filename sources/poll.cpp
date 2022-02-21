#include "poll.hpp"

#include <iostream>
#include <unistd.h>

#include "utils/strings.hpp"

#include "global.hpp"
#include "client.hpp"

/**
 * @brief polling loop
 * 
 * @param global 
 */
void	ft_poll(t_global& global)
{
	fd_set		pollset;
	std::string	packet;

	while (true) 
	{
		FD_ZERO(&pollset);
		FD_SET(global.server.file, &pollset);

		for (size_t i = 0; i < global.clients.size(); i++)
			FD_SET(global.clients[i].socket.file, &pollset);

		if (select(FD_SETSIZE, &pollset, NULL, NULL, NULL) == -1)
			throw Exception(strerror(errno));	

		if (FD_ISSET(global.server.file, &pollset))
		{
			t_socket socket = ft_saccept(global.server);
			Client client(global, socket);
			global.clients.push_back(client);
			client.onConnect();
			continue ;
		}

		for (size_t i = 0; i < global.clients.size(); i++)
		{
			if (!FD_ISSET(global.clients[i].socket.file, &pollset))
				continue ;
			if (!ft_sread(global.clients[i].socket, &packet))
			{
				global.clients[i].onDisconnect();
				global.clients.erase(global.clients.begin() + i);
				break ;
			}
			global.clients[i].onPacket(packet);
			break ;
		}
	}
}
