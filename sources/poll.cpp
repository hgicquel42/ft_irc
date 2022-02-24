#include "poll.hpp"

#include <iostream>
#include <unistd.h>

#include "utils/strings.hpp"
#include "utils/packet.hpp"
#include "utils/colors.hpp"

#include "global.hpp"
#include "client.hpp"

using namespace std;

/**
 * @brief polling loop
 * 
 * @param global 
 */
void	ft_poll(t_global& global)
{
	fd_set		pollset;
	string	packet;

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
			Client* client = global.clients[i];
			if (!FD_ISSET(client->socket.file, &pollset))
				continue ;
			if (!ft_sread(client->socket, packet))
			{
				client->onQuit("Connection lost");
				break ;
			}
			try {
				vector<string> packets = ft_split(packet);
				for (size_t j = 0; j < packets.size(); j++)
					client->onPacket(ft_unpack(packets[j]));
			} catch (exception& e){
				if (Numeric* e2 = dynamic_cast<Numeric*>(&e))
					client->write(e.what());
				else if (Exception* e2 = dynamic_cast<Exception*>(&e))
					client->onError(e.what());
				else
					throw e;
			}
			break ;
		}
	}
	// TODO: free clients et channels
}
