#include "poll.hpp"

#include <iostream>
#include <unistd.h>

#include "utils/strings.hpp"
#include "utils/packet.hpp"
#include "utils/colors.hpp"

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
			FD_SET(global.clients[i]->socket.file, &pollset);

		if (select(FD_SETSIZE, &pollset, NULL, NULL, NULL) == -1)
			throw Exception(strerror(errno));	

		if (FD_ISSET(global.server.file, &pollset))
		{
			t_socket socket = ft_saccept(global.server);
			Client* client = new Client(global, socket);
			global.clients.push_back(client);
			client->onConnect();
			continue ;
		}

		for (size_t i = 0; i < global.clients.size(); i++)
		{
			if (!FD_ISSET(global.clients[i]->socket.file, &pollset))
				continue ;
			if (!ft_sread(global.clients[i]->socket, packet))
			{
				global.clients[i]->onDisconnect();
				break ;
			}
			try {
				std::vector<std::string> v = ft_split(packet);
				for (size_t j = 0; j < v.size(); j++)
					global.clients[i]->onPacket(ft_unpack(v[j]));
			} catch (std::exception& e){
				std::cerr << ft_red(e.what()) << std::endl;
			}
			break ;
		}
	}
	// TODO: free clients
}
