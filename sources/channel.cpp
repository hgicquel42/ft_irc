#include "channel.hpp"

#include <iostream>

#include "utils/vector.hpp"

using namespace std;

Channel::Channel(const string& name):
	name(name),
	invite(false),
	closed(false)
{}

Channel::Channel(const Channel& from):
	name(from.name),
	password(from.password),
	invite(from.invite),
	closed(from.closed),
	clients(from.clients),
	invites(from.invites),
	banlist(from.banlist)
{}

Channel::~Channel(void)
{}

Channel&	Channel::operator=(const Channel& from)
{
	if (this == &from)
		return (*this);
	this->name = from.name;
	this->password = from.password;
	this->invite = from.invite;
	this->closed = from.closed;
	this->invites = from.invites;
	this->banlist = from.banlist;
	this->clients = from.clients;
	return (*this);
}

Channel* Channel::find(MChannels& channels, const string& name)
{
	MChannels::iterator it = channels.find(name);
	if (it != channels.end())
		return (it->second);
	return (NULL);
}

Channel*	Channel::findOrCreate(MChannels& channels, const string& name)
{
	MChannels::iterator it = channels.find(name);
	if (it != channels.end())
		return (it->second);
	Channel* channel = new Channel(name);
	channels[name] = channel;
	return (channel);
}

bool	Channel::isname(const string& name)
{
	if (name.size() > 50)
		return (false);
	if (name.find(",") != string::npos)
		return (false);
	if (name.find(":") != string::npos)
		return (false);
	if (name.rfind("#", 0) == 0)
		return (true);
	if (name.rfind("&", 0) == 0)
		return (true);
	if (name.rfind("+", 0) == 0)
		return (true);
	if (name.rfind("!", 0) == 0)
		return (true);
	return (false);
}