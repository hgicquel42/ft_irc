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

void	Client::getInfos(std::string packet)
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
		return ;
	}

	if (packet.rfind("USER ", 0) == 0)
	{
		// USER brmasser brmasser 127.0.0.1 :Bryce MASSERON
		// https://stackoverflow.com/questions/31666247/what-is-the-difference-between-the-nick-username-and-real-name-in-irc-and-wha

		std::vector<std::string> v = ft_splitby(packet, ':');
		if (v.size() != 2)
			throw Exception("Invalid USER packet");
		this->realname = v[1];

		std::vector<std::string> v2 = ft_splitby(v[0], ' ');
		if (v2.size() != 4)
			throw Exception("Invalid USER packet");
		this->username = v2[1];
		this->hostname = v2[2];
		return ;
	}

	throw Exception("Unknown packet");
}

void Client::onPacket(std::string packet)
{
	std::cout << packet << std::endl;

	if (this->state == REGISTERING)
	{
		getInfos(packet);
		
	}

	if (this->state == CONNECTED)
	{

	}

	throw Exception("Invalid state");
}