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

void Client::onPacket(std::string packet)
{
	std::cout << packet << "\n";

	if (step == 0)
	{
		// get CAP LS
		this->step++;
	}
	
	if (step == 5)
	{
		// recoit un message
	}
}