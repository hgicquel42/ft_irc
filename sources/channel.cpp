#include "channel.hpp"

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
	invites(from.invites)
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
	this->clients = from.clients;
	return (*this);
}

Channel* Channel::find(t_global& global, const string& name)
{
	MChannels::iterator it = global.channels.find(name);
	if (it != global.channels.end())
		return (it->second);
	return (NULL);
}

Channel*	Channel::findOrCreate(t_global& global, const string& name)
{
	MChannels::iterator it = global.channels.find(name);
	if (it != global.channels.end())
		return (it->second);
	Channel* channel = new Channel(name);
	global.channels[name] = channel;
	return (channel);
}
