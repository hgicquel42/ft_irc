#include "client.hpp"

#include <iostream>
#include <unistd.h>
#include <vector>

#include "utils/strings.hpp"

#include "global.hpp"

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

void Client::onConnect(void)
{
	std::cout << "Connected\n";
	// TODO: envoyer le MOTD
}

void Client::onDisconnect(void)
{
	std::cout << "Disconnected\n";
	close(this->socket.file);
}

void	Client::onRegisterPacket(std::string packet)
{
	if (packet == "CAP LS")
	{
		// TODO: renvoyer le bon packet
		return ;
	}

	if (packet.rfind("NICK ", 0) == 0)
	{
		std::vector<std::string> v = ft_splitby(packet, ' ');
		if (v.size() != 2)
			throw Exception("Invalid NICK packet");
		this->nickname = v[1];
		// TODO: checker si le nick n'est pas deja utilise par un autre
		return ;
	}
	
	if (packet.rfind("PASS ", 0) == 0)
	{
		if (packet != "PASS " + this->global.params.password)
			throw Exception("Invalid PASS packet");
		this->steps.pass = true;
		return ;
	}

	if (packet.rfind("USER ", 0) == 0)
	{
		std::vector<std::string> v = ft_splitby(packet, ':');
		if (v.size() != 2)
			throw Exception("Invalid USER packet");
		this->realname = v[1];

		std::vector<std::string> v2 = ft_splitby(v[0], ' ');
		if (v2.size() != 4)
			throw Exception("Invalid USER packet");
		this->username = v2[1];
		this->hostname = v2[2];

		this->steps.user = true;
		return ;
	}

	throw Exception("Unknown packet");
}

void Client::onPacket(std::string packet)
{
	if (this->state == REGISTERING)
	{
		this->onRegisterPacket(packet);
		if (!this->steps.user)
			return ;
		if (!this->steps.pass)
			return ;
		std::cout << "Registered" << std::endl;
		this->state = CONNECTED;
		return ;
	}

	if (this->state == CONNECTED)
	{
		std::cout << packet << std::endl;
		return ;
	}

	throw Exception("Invalid state");
}