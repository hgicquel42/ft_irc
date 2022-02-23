#include "channel.hpp"

using namespace std;

Channel::Channel(const string& name):
	name(name),
	inviteonly(false),
	passonly(false)
{}

Channel::Channel(const Channel& from):
	name(from.name),
	pass(from.pass),
	inviteonly(from.inviteonly),
	passonly(from.passonly)
{}

Channel::~Channel(void)
{}

Channel&	Channel::operator=(const Channel& from)
{
	if (this == &from)
		return (*this);
	this->name = from.name;
	this->pass = from.pass;
	this->inviteonly = from.inviteonly;
	this->passonly = from.passonly;
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
