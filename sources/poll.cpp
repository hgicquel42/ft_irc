#include "poll.hpp"

#include <iostream>
#include <unistd.h>
#include <signal.h>

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

	while (global.running) 
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
			string buffer = ft_sread(client->socket);
			if (buffer.empty())
			{
				client->onQuit("Connection lost");
				break ;
			}
			client->buffer += buffer;
			if (buffer[buffer.size() - 1] != '\n')
				break ;
			try {
				vector<string> packets = ft_split(client->buffer);
				client->buffer.erase();
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
}
