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

#include "errors.hpp"

using namespace std;

typedef struct s_socket
{
	int 				file;
	struct sockaddr_in	addr;
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

/**
 * @brief safely read socket
 * 
 * @param socket 
 * @return string 
 */
string	ft_sread(t_socket socket) throw(Exception);