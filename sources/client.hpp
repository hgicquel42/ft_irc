#pragma once

typedef struct s_global t_global;

#include "utils/sockets.hpp"

#include "global.hpp"

class Client
{
	private:
		t_global&	global;

	public:
		t_socket	socket;
		int			step;
		std::string	nickname;
		std::string	realname;
		std::string	username;

		Client(t_global& global, t_socket socket);
		Client(const Client& from);
		~Client(void);

		Client&	operator=(const Client& from);

		void	onConnect(void);
		void	onDisconnect(void);
		void	onPacket(std::string packet);
		int		getInfos(std::string str);
};