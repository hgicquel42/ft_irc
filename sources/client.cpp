#include "client.hpp"

#include <iostream>
#include <unistd.h>

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

int	Client::getInfos(int *ptr_step, std::string str, t_global &global)
{
	if (*ptr_step == 0)
	{
		if (str == "CAP LS")
			(*ptr_step)++;
	}
	else if (*ptr_step == 1)
	{
		if (str == "PASS " + global.params.password)
			(*ptr_step)++;
	}
	else if (*ptr_step == 2)
	{
		if (str.compare(0, 5, "NICK ") == 0)
		{
			this->nick = (str.substr(5, str.length() - 5));
			(*ptr_step)++;
		}
	}
	//USER brmasser brmasser 127.0.0.1 :Bryce MASSERON
	else if (*ptr_step == 3)
	{
		if (str.compare(0, 5, "USER ") == 0)
		{
			std::vector<std::string> tab;
			tab = ft_split(str);
			 
		}
	}
	else
		return (-1);
	return (0);
}

void Client::onPacket(std::string packet, t_global &global)
{
	std::cout << packet << "\n";

	if (step >= 0 && step < 4)
	{
		getInfos(&step, packet, global);
		this->step++;
	}
}