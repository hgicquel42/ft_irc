#include "client.hpp"

#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "utils/strings.hpp"
#include "utils/vector.hpp"
#include "utils/packet.hpp"
#include "utils/set.hpp"

#include "global.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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

void	Client::write(const std::string& packet)
{
	std::string line(packet + "\r\n");
	std::cout << "-> " << packet << std::endl;

	if (send(this->socket.file, line.c_str(), line.length(), 0) == -1)
		throw Exception(strerror(errno));
}

void	Client::onConnect(void)
{
	std::cout << "Connected\n";
}

void	Client::onDisconnect(void)
{
	std::cout << "Disconnected\n";
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
		std::string nickname = packet.args[1];
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
	if (!ft_setexists(this->channels, packet.args[1]))
		throw Exception("User is not in channel");
	for (size_t i = 0; i < this->global.clients.size(); i++)
	{
		if (this->global.clients[i] == this)
			continue ;
		if (!ft_setexists(this->global.clients[i]->channels, packet.args[1]))
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
	for (size_t i = 0; i < this->global.clients.size(); i++)
	{
		if (this->global.clients[i] == this)
			continue ;
		if (this->global.clients[i]->nickname != packet.args[1])
			continue ;
		this->global.clients[i]->write(":" + this->nickname + " NOTICE " + packet.args[1] + " :" + packet.rest);
	}
}

void	Client::onRegularPacket(const t_packet& packet)
{
	std::cout << packet.raw << std::endl;
	
	if (packet.args[0] == "QUIT")
	{
		this->onDisconnect();
		return ;
	}

	if (packet.args[0] == "JOIN")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid JOIN packet");
		this->channels.insert(packet.args[1]);
		return ;
	}

	if (packet.args[0] == "PRIVMSG")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid PRIVMSG packet");
		if (packet.args[1].rfind("#", 0) == 0)
			this->onChannelMessagePacket(packet);
		else
			this->onPrivateMessagePacket(packet);
		return ;
	}

	if (packet.args[0] == "NOTICE")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid NOTICE packet");
		this->onNoticePacket(packet);
		return ;
	}

	if (packet.args[0] == "NICK")
	{
		if (packet.args.size() != 2)
			throw Exception("Invalid NICK packet");
		std::string nickname = packet.args[1];
		for (size_t i = 0; i < this->global.clients.size(); i++)
			if (this->global.clients[i]->nickname == nickname)
				throw Exception("This nickname already exists");
		this->nickname = nickname;
		return ;
	}

	if (packet.args[0] == "OPER")
	{
		if (packet.args.size() != 3)
			throw Exception("Invalid OPER packet");
		if (packet.args[2] != this->global.params.password)
			throw Exception("Invalid password");
		this->opped = true;
		return ;
	}

	if (packet.args[0] == "KICK")
	{
		if (packet.args.size() != 3)
			throw Exception("Invalid KICK packet");
		if (!this->opped)
			throw Exception("Not operator");
		for (size_t i = 0; i < this->global.clients.size(); i++)
		{
			if (this->global.clients[i]->nickname != packet.args[2])
				continue ;
			if (!ft_setexists(this->global.clients[i]->channels, packet.args[1]))
				throw Exception("");
			this->global.clients[i]->channels.erase(packet.args[1]);
			this->global.clients[i]->write(":" + this->nickname + " KICK " + packet.args[1] + " " + packet.args[2] + " :" + packet.rest);
			return ;
		}
		throw Exception("Unknown nickname or channel");
	}

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

	/*
///mode #channel +b nick
	if (packet.args[0] == "MODE")
	{
		if (packet.args.size() < 3 || packet.args.size() > 4)
			throw Exception("Invalid MODE packet");
		if (packet.args[1].rfind("#", 0) == 0)
		{

		}
	}
	*/

}

void	Client::motd(void)
{
	std::ifstream file(std::string("motd.txt").c_str(), std::ios::binary);
	if (!file)
		throw Exception("MOTD not found");
	std::stringstream buffer;
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
		std::cout << "Registered" << std::endl;
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