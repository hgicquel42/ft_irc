#pragma once

typedef struct s_global t_global;

#include "utils/sockets.hpp"

#include "global.hpp"

#define REGISTERING 0
#define CONNECTED 1
#define DISCONNECTED 2

typedef struct	s_steps
{
	bool	nick;
	bool	pass;
	bool 	caps;
	bool	user;
}	t_steps;

class Client
{
	private:
		t_global&	global;

	public:
		t_socket	socket;
		t_steps		steps;
		int			state;
		
		std::string	nickname;
		std::string	realname;
		std::string	username;
		std::string	hostname;

		Client(t_global& global, t_socket socket);
		Client(const Client& from);
		~Client(void);

		Client&	operator=(const Client& from);

		void	onConnect(void);
		void	onDisconnect(void);
		void	onPacket(std::string packet);

		void	onRegisterPacket(std::string packet);
};