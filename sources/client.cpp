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

#include "global.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

Client::Client(t_global& global, t_socket socket):
	global(global),
	socket(socket),
	state(REGISTERING)
{}

Client::Client(const Client& from):
	global(from.global),
	socket(from.socket),
	state(from.state)
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
	return (*this);
}

void	Client::write(const std::string& packet)
{
	if (send(this->socket.file, packet.c_str(), packet.length(), 0) == -1)
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
}

void	Client::onRegisterPacket(const t_packet& packet)
{
	if (packet.raw == "CAP LS")
	{
		this->write("CAP * LS :\r\n");
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
		this->realname = packet.rest;
		this->username = packet.args[1];
		this->hostname = packet.args[2];

		this->steps.user = true;
		return ;
	}

	throw Exception("Unknown packet");
}

void	Client::onRegularPacket(const t_packet& packet)
{
	std::cout << packet.raw << std::endl;
	
	if (packet.args[0] == "QUIT")
	{
		this->onDisconnect();
	}
}

void	Client::motd(void)
{
	std::cout << "Welcome to the GigaChadIRC server.\n";

	std::ifstream file(std::string("motd.txt").c_str(), std::ios::binary);
	if (!file)
		throw Exception("MOTD not found");
	std::stringstream buffer;
	buffer << file.rdbuf();
	this->write("PRIVMSG lol :hello world\r\n");
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