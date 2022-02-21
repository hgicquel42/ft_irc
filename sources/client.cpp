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

int	Client::getInfos(std::string str)
{
	if (this->step == 0)
	{
		if (str.compare("CAP LS") == 0)
			this->step++;
		else
			std::cout << "COUCOU>" << str << "<CACA" << std::endl;
	}
	else if (this->step == 1)
	{
		if (str == "PASS " + this->global.params.password)
			this->step++;
	}
	else if (this->step == 2)
	{
		if (str.compare(0, 5, "NICK ") == 0)
		{
			this->nickname = (str.substr(5, str.length() - 5));
			this->step++;
		}
	}
	//USER brmasser brmasser 127.0.0.1 :Bryce MASSERON
	else if (this->step == 3)
	{
		if (str.compare(0, 5, "USER ") == 0)
		{
			std::vector<std::string> tab;
			tab = ft_split(str, ':');
			if (tab.size() != 2)
				throw Exception("Invalid USER packet");
			this->realname = tab[1];
			tab = ft_split(tab[0], ' ');
			if (tab.size() != 4)
				throw Exception("Invalid USER packet");
			this->username = tab[1];
		}
	}
	else
		return (-1);
	return (0);
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