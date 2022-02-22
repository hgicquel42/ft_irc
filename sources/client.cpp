#include "client.hpp"

#include <iostream>
#include <unistd.h>
#include <vector>

#include "utils/strings.hpp"

#include "global.hpp"

Client::Client(t_global& global, t_socket socket):
	global(global),
	socket(socket),
	step(0)
{}

Client::Client(const Client& from):
	global(from.global),
	socket(from.socket),
	step(from.step)
{}

Client::~Client(void)
{}

Client&	Client::operator=(const Client& from)
{
	if (this == &from)
		return (*this);
	this->global = from.global;
	this->socket = from.socket;
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

void	Client::getInfos(std::string packet)
{
	if (this->step == 0)
	{
		if (packet != "CAP LS")
			throw Exception("Unknown packet");
		this->step++;
		return ;
	}
	if (this->step == 1)
	{
		if (packet.rfind("PASS ", 0) != 0)
			throw Exception("Unknown packet");
		if (packet != "PASS " + this->global.params.password)
			throw Exception("Invalid packet");
		this->step++;
		return ;
	}
	if (this->step == 2)
	{
		if (packet.rfind("NICK ", 0) != 0)
			throw Exception("Unknown packet");
		this->nickname = (packet.substr(5, packet.length() - 5));
		this->step++;
	}
	if (this->step == 3)
	{
		if (packet.rfind("USER ", 0) != 0)
			throw Exception("Unknown packet");
		//USER brmasser brmasser 127.0.0.1 :Bryce MASSERON
		std::vector<std::string> tab;
		tab = ft_split(packet, ':');
		if (tab.size() != 2)
			throw Exception("Invalid USER packet");
		this->realname = tab[1];
		tab = ft_split(tab[0], ' ');
		if (tab.size() != 4)
			throw Exception("Invalid USER packet");
		this->username = tab[1];
		this->step++;
		return ;
	}
}

void Client::onPacket(std::string packet)
{
	std::cout << packet << "\n";
	std::cout << "step = " << this->step << std::endl;

	if (step >= 0 && step < 4)
		getInfos(packet);
	else if (step == 4)
		std::cout << "Congrats biatch, step = 4" << std::endl;
	std::cout << "end packet" << std::endl;
}