#include "sockets.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

/**
 * @brief listen to port
 * 
 * @param port 
 * @return t_socket 
 */
t_socket	ft_listen(int port) throw(Exception)
{
	int			options = 1;
	t_socket	server;

	server.socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server.socket == -1)
		throw Exception(std::strerror(errno));
	if (setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options, sizeof(options)) == -1)
		throw Exception(std::strerror(errno));
	server.address.sin_family = AF_INET;
    server.address.sin_addr.s_addr = INADDR_ANY;
    server.address.sin_port = htons(port);
	if (bind(server.socket, (struct sockaddr *) &server.address, sizeof(server.address)) == -1)
		throw Exception(std::strerror(errno));
	if (listen(server.socket, 3) == -1)
		throw Exception(std::strerror(errno));
	return (server);
}

/**
 * @brief accept a client
 * 
 * @param server 
 * @return t_socket 
 */
t_socket	ft_accept(t_socket server) throw(Exception)
{
	t_socket	client;
	socklen_t	length;

	client.socket = accept(server.socket, (struct sockaddr *) &client.address, &length);
	if (!client.socket)
		throw Exception(std::strerror(errno)); 
	return (client);
}