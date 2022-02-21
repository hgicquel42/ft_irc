#pragma once

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "utils/errors.hpp"

typedef struct s_socket
{
	int 				socket;
	struct sockaddr_in	address;
}	t_socket;

/**
 * @brief listen to port
 * 
 * @param port 
 * @return t_socket 
 */
t_socket	ft_slisten(int port) throw(Exception);

/**
 * @brief accept a client
 * 
 * @param server 
 * @return t_socket 
 */
t_socket	ft_saccept(t_socket server) throw(Exception);