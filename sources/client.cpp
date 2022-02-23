#include "client.hpp"

#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "utils/colors.hpp"
#include "utils/strings.hpp"
#include "utils/vector.hpp"
#include "utils/packet.hpp"

#include "global.hpp"
#include "channel.hpp"

using namespace std;

Client::Client(t_global& global, t_socket socket):
	global(global),
	socket(socket),
	state(REGISTERING),
	opped(false)
{}

Client::Client(const Client& from):
	global(from.global),
	socket(from.socket),
	state(from.state),
	opped(from.opped)
{}

Client::~Client(void)
{}

Client&	Client::operator=(const Client& from)
{
	if (this == &from)
		return (*this);
	this->global = from.global;
	this->socket = from.socket;
	this->state = from.state;
	this->steps = from.steps;
	this->nickname = from.nickname;
	this->username = from.username;
	this->hostname = from.hostname;
	this->realname = from.realname;
	this->opped = from.opped;
	this->channels = from.channels;
	return (*this);
}

void	Client::disconnect(const string& reason)
{
	cerr << ft_red(reason) << endl;
	this->onDisconnect();
}

void	Client::write(const string& packet)
{
	string line(packet + "\r\n");
	cout << "-> " << packet << endl;

	if (send(this->socket.file, line.c_str(), line.length(), 0) == -1)
		throw Exception(strerror(errno));
}

void	Client::onConnect(void)
{
	cout << "Connected\n";
}

void	Client::onDisconnect(void)
{
	cout << "Disconnected\n";
	for (size_t i = 0; i < this->channels.size(); i++)
		this->global.channels[this->channels[i]];
	ft_vecrem(this->global.clients, this);
	close(this->socket.file);
	delete this;
}

void	Client::onRegisterPacket(const t_packet& packet)
{
	if (packet.raw == "CAP LS")
	{
		this->write("CAP * LS :");
		this->steps.caps = true;
		return ;
	}

	if (packet.raw == "CAP END")
	{
		this->steps.caps = false;
		return ;
	}

	if (packet.args[0] == "NICK")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid NICK packet");
		string nickname = packet.args[1];
		for (size_t i = 0; i < this->global.clients.size(); i++)
			if (this->global.clients[i]->nickname == nickname)
				throw Exception("This nickname already exists");
		this->nickname = nickname;
		this->steps.nick = true;
		return ;
	}
	
	if (packet.args[0] == "PASS")
	{
		if (packet.args[1] != this->global.params.password)
			throw Exception("Invalid PASS packet");
		this->steps.pass = true;
		return ;
	}

	if (packet.args[0] == "USER")
	{
		if (packet.args.size() != 4)
			throw Exception("Invalid USER packet");
		this->username = packet.args[1];
		this->hostname = packet.args[2];
		this->realname = packet.rest;

		this->steps.user = true;
		return ;
	}

	throw Exception("Unknown packet");
}

void	Client::onChannelMessagePacket(const t_packet& packet)
{
	
	if (!ft_vecexists(this->channels, packet.args[1]))
		throw Exception("User is not in channel");
	for (size_t i = 0; i < this->global.clients.size(); i++)
	{
		if (this->global.clients[i] == this)
			continue ;
		if (!ft_vecexists(this->global.clients[i]->channels, packet.args[1]))
			continue ;
		this->global.clients[i]->write(":" + this->nickname + " PRIVMSG " + packet.args[1] + " :" + packet.rest);
	}
}

void	Client::onPrivateMessagePacket(const t_packet& packet)
{
	for (size_t i = 0; i < this->global.clients.size(); i++)
	{
		if (this->global.clients[i] == this)
			continue ;
		if (this->global.clients[i]->nickname != packet.args[1])
			continue ;
		this->global.clients[i]->write(":" + this->nickname + " PRIVMSG " + packet.args[1] + " :" + packet.rest);
	}
}

void	Client::onNoticePacket(const t_packet& packet)
{
	if (packet.args.size() != 2)
		throw Exception("Invalid NOTICE packet");
	for (size_t i = 0; i < this->global.clients.size(); i++)
	{
		if (this->global.clients[i] == this)
			continue ;
		if (this->global.clients[i]->nickname != packet.args[1])
			continue ;
		this->global.clients[i]->write(":" + this->nickname + " NOTICE " + packet.args[1] + " :" + packet.rest);
	}
}

void	Client::onJoinPacket(const t_packet& packet)
{
	if (packet.args.size() != 2)
		throw Exception("Invalid JOIN packet");
	Channel* channel = Channel::findOrCreate(this->global, packet.args[1]);
	channel->clients.push_back(this);
	this->channels.push_back(packet.args[1]);
}

void	Client::onQuitPacket(const t_packet& packet)
{
	(void)packet;
	// TODO: afficher le message
	this->onDisconnect();
}

void	Client::onNickPacket(const t_packet& packet)
{
	if (packet.args.size() != 2)
		throw Exception("Invalid NICK packet");
	string nickname = packet.args[1];
	for (size_t i = 0; i < this->global.clients.size(); i++)
		if (this->global.clients[i]->nickname == nickname)
			throw Exception("This nickname already exists");
	this->nickname = nickname;
}

void	Client::onOperPacket(const t_packet& packet)
{
	if (packet.args.size() != 3)
		throw Exception("Invalid OPER packet");
	if (packet.args[2] != this->global.params.password)
		throw Exception("Invalid password");
	this->opped = true;
}

void	Client::onKickPacket(const t_packet& packet)
{
	if (packet.args.size() != 3)
		throw Exception("Invalid KICK packet");
	if (!this->opped)
		throw Exception("Not operator");
	for (size_t i = 0; i < this->global.clients.size(); i++)
	{
		if (this->global.clients[i]->nickname != packet.args[2])
			continue ;
		if (!ft_vecexists(this->global.clients[i]->channels, packet.args[1]))
			throw Exception("Channel doesn't exist");
		ft_vecrem(this->global.clients[i]->channels, packet.args[1]);
		this->global.clients[i]->write(":" + this->nickname + " KICK " + packet.args[1] + " " + packet.args[2] + " :" + packet.rest);
		return ;
	}
	throw Exception("Unknown nickname or channel");
}

void	Client::onModePacket(const t_packet& packet)
{
	if (packet.args.size() < 3 || packet.args.size() > 4)
		throw Exception("Invalid MODE packet");
	if (!this->opped)
		throw Exception("Not operator");
	if (packet.args[1] == "*")
		return ;
	Channel* channel = Channel::find(this->global, packet.args[1]);
	if (!channel)
		throw Exception("Channel doesn't exists");

	if (channel->pass.empty())
	{
		if (packet.args[2] == "+i")
		{
			if (packet.args.size() != 3)
				throw Exception("Invalid MODE packet");
			channel->inviteonly = true;
			this->write(":" + this->nickname + " 324 " + this->nickname + " " + packet.args[1] + " +ni");
		}
		if (packet.args[2] == "-i")
		{
			if (packet.args.size() != 3)
				throw Exception("Invalid MODE packet");
			channel->inviteonly = false;
			this->write(":" + this->nickname + " 324 " + this->nickname + " " + packet.args[1] + " +n");
		}
		if (packet.args[2] == "+k")
		{
			if (packet.args.size() != 4)
				throw Exception("Invalid MODE packet");
			channel->pass = packet.args[3];
			this->write(":" + this->nickname + " 324 " + this->nickname + " " + packet.args[1] + " +nki");
		}
	}
	else
	{
		if (packet.args[2] == "+i")
		{
			if (packet.args.size() != 3)
				throw Exception("Invalid MODE packet");
			channel->inviteonly = true;
			this->write(":" + this->nickname + " 324 " + this->nickname + " " + packet.args[1] + " +nki");
		}
		if (packet.args[2] == "-i")
		{
			if (packet.args.size() != 3)
				throw Exception("Invalid MODE packet");
			channel->inviteonly = false;
			this->write(":" + this->nickname + " 324 " + this->nickname + " " + packet.args[1] + " +nk");
		}
		if (packet.args[2] == "-k")
		{
			if (packet.args.size() != 3)
				throw Exception("Invalid MODE packet");
			channel->pass.erase();
			if (!channel->inviteonly)
				this->write(":" + this->nickname + " 324 " + this->nickname + " " + packet.args[1] + " +n");
			else
				this->write(":" + this->nickname + " 324 " + this->nickname + " " + packet.args[1] + " +ni");
		}
	}
}

void	Client::onRegularPacket(const t_packet& packet)
{
	cout << "<- " << packet.raw << endl;
	
	if (packet.args[0] == "QUIT")
		return (this->onQuitPacket(packet));

	if (packet.args[0] == "JOIN")
		return (this->onJoinPacket(packet));

	if (packet.args[0] == "NOTICE")
		return (this->onNoticePacket(packet));

	if (packet.args[0] == "NICK")
		return (this->onNickPacket(packet));

	if (packet.args[0] == "OPER")
		return (this->onOperPacket(packet));

	if (packet.args[0] == "MODE")
		return (this->onModePacket(packet));

	if (packet.args[0] == "PRIVMSG")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid PRIVMSG packet");
		if (packet.args[1].rfind("#", 0) == 0)
			return (this->onChannelMessagePacket(packet));
		else
			return (this->onPrivateMessagePacket(packet));
	}

	if (packet.args[0] == "KICK")
		return (this->onKickPacket(packet));

	if (packet.args[0] == "INVITE")
	{
		if (packet.args.size() != 3)
			throw Exception("Invalid INVITE packet");
		for (size_t i = 0; i < this->global.clients.size(); i++)
		{
			if (this->global.clients[i] == this)
				continue ;
			if (this->global.clients[i]->nickname != packet.args[1])
				continue;
			this->global.clients[i]->write(":" + this->nickname + " INVITE " + packet.args[1] + " " + packet.args[2]);
			return ;
		}
		throw Exception("Unknown nickname"); 
	}

	//MODE #chan +k admin -> 4

	// 	<- OPER admin admin
	// -> :bbb!hgicquel@localhost 381 bbb :You are now an IRC operator
	// -> :bbb!hgicquel@localhost 221 bbb +wio
	// <- MODE #lol +k
	// -> :bbb!hgicquel@localhost 461 bbb MODE :Not enough parameters
	// -> :bbb!hgicquel@localhost 324 bbb #lol +n 
	// <- MODE #lol +k admin
	// -> PING bbb
	// -> :bbb!hgicquel@localhost 324 bbb #lol +nk admin 
	// <- PONG :bbb
	// <- MODE #lol -k admin
	// -> :bbb!hgicquel@localhost 324 bbb #lol +n 

	// <- MODE #lol +i
	// -> :bbb!hgicquel@localhost 324 bbb #lol +ni 
	// <- MODE #lol -i
	// -> PING bbb
	// -> :bbb!hgicquel@localhost 324 bbb #lol +n 
	// <- PONG :bbb
	// -> :aaa!hgicquel@localhost JOIN :#lol
	// <- PING bbb
	// -> :bbb!hgicquel@localhost PONG :bbb


//mode #channel +i -> rend le channel invite only (peut etre rejoint only si t'as ete invited)
//mode #channel -i -> unset l'invite only mode
//mode #channel +k -> set a channel key (password)
//mode #channel -k -> unset a channel key (password)


//mode #channel +o nick -> add to the operator list
//mode #channel -o nick -> remove list operator
//mode #channel +b nick


	if (packet.args[0] == "TIME")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid TIME packet");
		this->write(this->nickname + " TIME " + "127.0.0.1");
			return ;
	}

	if (packet.args[0] == "INFO")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid INFO packet");
		this->write("371 :");
		this->write("374 :ENDOFINFO");
		return ;
	}

	if (packet.args[0] == "ADMIN")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid ADMIN packet");
		this->write(":" + this->nickname + " ADMIN " + "127.0.0.1");
			return ;
	}

	if (packet.args[0] == "VERSION")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid VERSION packet");
		this->write(":" + this->nickname + " VERSION " + "127.0.0.1");
			return ;
	}


}

void	Client::motd(void)
{
	ifstream file(string("motd.txt").c_str(), ios::binary);
	if (!file)
		throw Exception("MOTD not found");
	stringstream buffer;
	buffer << file.rdbuf();
}

void	Client::onPacket(const t_packet& packet)
{
	if (this->state == REGISTERING)
	{
		this->onRegisterPacket(packet);
		if (this->steps.caps)
			return ;
		if (!this->steps.pass)
			return ;
		if (!this->steps.nick)
			return ;
		if (!this->steps.user)
			return ;
		cout << "Registered" << endl;
		this->state = REGISTERED;
		this->write("001 * :Hello world!");
		this->motd();
		return ;
	}

	if (this->state == REGISTERED)
	{
		this->onRegularPacket(packet);
		return ;
	}

	throw Exception("Invalid state");
}