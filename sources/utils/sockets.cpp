#include "sockets.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "strings.hpp"

/**
 * @brief listen to port
 * 
 * @param port 
 * @return t_socket 
 */
t_socket	ft_slisten(int port) throw(Exception)
{
	int			options = 1;
	t_socket	server;

	server.file = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (server.file == -1)
		throw Exception(std::strerror(errno));
	if (setsockopt(server.file, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options, sizeof(options)) == -1)
		throw Exception(std::strerror(errno));
	server.addr.sin_family = AF_INET;
    server.addr.sin_addr.s_addr = INADDR_ANY;
    server.addr.sin_port = htons(port);
	if (bind(server.file, (struct sockaddr *) &server.addr, sizeof(server.addr)) == -1)
		throw Exception(std::strerror(errno));
	if (listen(server.file, 3) == -1)
		throw Exception(std::strerror(errno));
	return (server);
}

/**
 * @brief accept a client
 * 
 * @param server 
 * @return t_socket 
 */
t_socket	ft_saccept(t_socket server) throw(Exception)
{
	t_socket	client;
	socklen_t	length = sizeof(client.addr);

	client.file = accept4(server.file, (struct sockaddr *) &client.addr, &length, SOCK_NONBLOCK);
	if (!client.file)
		throw Exception(std::strerror(errno)); 
	return (client);
}

/**
 * @brief securely read from socket
 * 
 * @param socket 
 * @param packet 
 * @return true 
 * @return false 
 */
bool	ft_sread(t_socket socket, std::string* packet) throw(Exception)
{
	char buffer[1024] = {0};

	int	n = read(socket.file, &buffer, 1024);
	if (n == -1)
		throw Exception("Could not read socket");
	if (n == 0)
		return (false);
	*packet = ft_strpop(buffer);
	return (true);
}