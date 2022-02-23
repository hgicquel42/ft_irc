#pragma once

#include <set>

typedef struct s_global t_global;

#include "utils/sockets.hpp"
#include "utils/packet.hpp"

#include "global.hpp"

#define REGISTERING 0
#define REGISTERED 1

using namespace std;

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
		
		string	nickname;
		string	realname;
		string	username;
		string	hostname;

		vector<string>	channels;
		bool			opped;

		Client(t_global& global, t_socket socket);
		Client(const Client& from);
		~Client(void);

		Client&	operator=(const Client& from);

		void	onConnect(void);
		void	onDisconnect(void);

		void	onPacket(const t_packet& packet);
		void	onRegisterPacket(const t_packet& packet);
		void	onRegularPacket(const t_packet& packet);
		
		void	onChannelMessagePacket(const t_packet& packet);
		void	onPrivateMessagePacket(const t_packet& packet);
		void	onNoticePacket(const t_packet& packet);
		void	onJoinPacket(const t_packet& packet);
		void	onQuitPacket(const t_packet& packet);
		void	onNickPacket(const t_packet& packet);
		void	onOperPacket(const t_packet& packet);
		void	onInvitePacket(const t_packet& packet);
		void	onKickPacket(const t_packet& packet);
		void	onModePacket(const t_packet& packet);

		void	write(const string& raw);

		void	motd(void);
};