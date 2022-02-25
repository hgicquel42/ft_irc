#include "client.hpp"

#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "utils/colors.hpp"
#include "utils/strings.hpp"
#include "utils/vector.hpp"
#include "utils/packet.hpp"

#include "global.hpp"
#include "channel.hpp"
#include "errors.hpp"
#include "replies.hpp"

using namespace std;

Client::Client(t_global& global, t_socket socket):
	global(global),
	socket(socket),
	state(REGISTERING),
	opped(false)
{
	this->steps.caps = false;
	this->steps.nick = false;
	this->steps.pass = false;
	this->steps.user = false;
}

Client::Client(const Client& from):
	global(from.global),
	socket(from.socket),
	steps(from.steps),
	state(from.state),
	nickname(from.nickname),
	realname(from.realname),
	username(from.username),
	hostname(from.hostname),
	channels(from.channels),
	opped(from.opped)
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
	this->steps = from.steps;
	this->opped = from.opped;
	this->nickname = from.nickname;
	this->username = from.username;
	this->hostname = from.hostname;
	this->realname = from.realname;
	this->channels = from.channels;
	return (*this);
}

Client*	Client::find(VClients& clients, const string& nickname)
{
	for (size_t i = 0; i < clients.size(); i++)
		if (clients[i]->nickname == nickname && clients[i]->state == REGISTERED)
			return (clients[i]);
	return (NULL);
}

void	Client::write(const string& packet)
{
	string line(packet + "\r\n");
	cout << "-> " << packet << endl;

	if (send(this->socket.file, line.c_str(), line.length(), 0) == -1)
		throw Exception(strerror(errno));
}

void	Client::onConnect(void)
{
	cout << "Connected\n";
}

void	Client::onDisconnect(void)
{
	cout << "Disconnected\n";

	for (size_t i = 0; i < this->channels.size(); i++)
		ft_vecrem(this->channels[i]->clients, this);
	ft_vecrem(this->global.clients, this);
	close(this->socket.file);
	delete this;
}

void	Client::onError(const string& reason)
{
	cerr << ft_red(reason) << endl;
	this->write(ERROR(reason));
	this->onDisconnect();
}

void	Client::onQuit(const string& reason)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		Channel* channel = this->channels[i];
		for (size_t j = 0; j < channel->clients.size(); j++)
		{
			Client* client = channel->clients[j];
			if (client == this)
				continue ;
			client->write(QUIT(this, reason));
		}
	}
	this->onDisconnect();
}

void	Client::onPingPacket(const t_packet& packet)
{
	if (packet.args.size() < 2)
		throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
	this->write(PONG(packet.args[1]));
}

void	Client::onQuitPacket(const t_packet& packet)
{
	this->onQuit("Quit: " + packet.rest);
}

void	Client::onRegisterPacket(const t_packet& packet)
{
	if (packet.raw == "CAP LS")
	{
		this->write("CAP * LS :");
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
		this->onNickPacket(packet);
		this->steps.nick = true;
		return ;
	}
	
	if (packet.args[0] == "PASS")
	{
		if (packet.args.size() < 2)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		if (packet.args[1] != this->global.params.password)
			throw Numeric(ERR_PASSWDMISMATCH(this));
		this->steps.pass = true;
		return ;
	}

	if (packet.args[0] == "USER")
	{
		if (packet.args.size() < 4)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		this->username = packet.args[1];
		this->hostname = packet.args[3];
		this->realname = packet.rest;

		this->steps.user = true;
		return ;
	}

	throw Numeric(ERR_UNKNOWNCOMMAND(this, packet));
}

void	Client::onChannelMessagePacket(const t_packet& packet)
{
	Channel* channel = Channel::find(this->global.channels, packet.args[1]);
	if (!channel)
		throw Numeric(ERR_NOSUCHCHANNEL(this, packet.args[1]));
	if (channel->closed && !ft_vecexists(channel->clients, this))
		throw Numeric(ERR_CANNOTSENDTOCHAN(this, channel));
	for (size_t i = 0; i < channel->clients.size(); i++)
	{
		Client* client = channel->clients[i];
		if (client == this)
			continue ;
		client->write(PRIVMSG(this, channel->name, packet.rest));
	}
}

void	Client::onPrivateMessagePacket(const t_packet& packet)
{
	Client* client = Client::find(this->global.clients, packet.args[1]);
	if (!client || client == this)
		throw Numeric(ERR_NOSUCHNICK(this, packet.args[1]));
	client->write(PRIVMSG(this, client->nickname, packet.rest));
}

void	Client::onMessagePacket(const t_packet& packet)
{
	if (packet.args.size() < 2)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
	if (Channel::isname(packet.args[1]))
		return (this->onChannelMessagePacket(packet));
	else
		return (this->onPrivateMessagePacket(packet));
}

void	Client::onChannelNoticePacket(const t_packet& packet)
{
	Channel* channel = Channel::find(this->global.channels, packet.args[1]);
	if (!channel)
		throw Numeric(ERR_NOSUCHCHANNEL(this, packet.args[1]));
	if (channel->closed && !ft_vecexists(channel->clients, this))
		throw Numeric(ERR_CANNOTSENDTOCHAN(this, channel));
	for (size_t i = 0; i < channel->clients.size(); i++)
	{
		Client* client = channel->clients[i];
		if (client == this)
			continue ;
		client->write(NOTICE(this, channel->name, packet.rest));
	}
}

void	Client::onPrivateNoticePacket(const t_packet& packet)
{
	Client* client = Client::find(this->global.clients, packet.args[1]);
	if (!client || client == this)
		throw Numeric(ERR_NOSUCHNICK(this, packet.args[1]));
	client->write(NOTICE(this, client->nickname, packet.rest));
}

void	Client::onNoticePacket(const t_packet& packet)
{
	if (packet.args.size() < 2)
		throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
	if (Channel::isname(packet.args[1]))
		return (this->onChannelNoticePacket(packet));
	else
		return (this->onPrivateNoticePacket(packet));
}

void	Client::onJoinPacket(const t_packet& packet)
{
	if (packet.args.size() < 2)
		throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
	if (!Channel::isname(packet.args[1]))
		return ;

	Channel* channel = Channel::findOrCreate(this->global.channels, packet.args[1]);

	if (channel->invite)
		if (!ft_vecexists(channel->invites, this->nickname))
			throw Numeric(ERR_INVITEONLYCHAN(this, channel));

	if (!channel->password.empty())
	{
		if (packet.args.size() < 3)
			throw Numeric(ERR_BADCHANNELKEY(this, channel));
		if (packet.args[2] != channel->password)
			throw Numeric(ERR_BADCHANNELKEY(this, channel));
	}

	if (ft_vecexists(channel->banlist, this->nickname))
		throw Numeric(ERR_BANNEDFROMCHAN(this, channel));

	channel->clients.push_back(this);
	this->channels.push_back(channel);

	for (size_t i = 0; i < channel->clients.size(); i++)
		channel->clients[i]->write(JOIN(this, channel));
}

void	Client::onNickPacket(const t_packet& packet)
{
	if (packet.args.size() < 2)
		throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		
	string nickname = packet.args[1];
	if (Client::find(this->global.clients, nickname))
		throw Numeric(ERR_NICKNAMEINUSE(this, nickname));
		
	for (size_t i = 0; i < this->global.clients.size(); i++)
		this->global.clients[i]->write(NICK(this, nickname));
	this->nickname = nickname;
}

void	Client::onOperPacket(const t_packet& packet)
{
	if (packet.args.size() < 3)
		throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
	if (packet.args[2] != this->global.params.password)
		throw Numeric(ERR_PASSWDMISMATCH(this));
	this->opped = true;
	this->write(RPL_YOUREOPER(this));
}

void	Client::onInvitePacket(const t_packet& packet)
{
	if (packet.args.size() < 3)
		throw Numeric(ERR_NEEDMOREPARAMS(this, packet));

	Channel* channel = Channel::find(this->global.channels, packet.args[2]);
	if (!channel)
		throw Numeric(ERR_NOSUCHCHANNEL(this, packet.args[2]));
	if (!ft_vecexists(channel->clients, this))
		throw Numeric(ERR_NOTONCHANNEL(this, channel));

	Client* client = Client::find(this->global.clients, packet.args[1]);
	if (!client || client == this)
		throw Numeric(ERR_NOSUCHNICK(this, packet.args[1]));
	if (ft_vecexists(channel->clients, client))
		throw Numeric(ERR_USERONCHANNEL(this, client, channel));

	channel->invites.push_back(client->nickname);
	client->write(INVITE(this, client, channel));
	this->write(RPL_INVITING(this, client, channel));
}

void	Client::onKickPacket(const t_packet& packet)
{
	if (packet.args.size() < 3)
		throw Numeric(ERR_NEEDMOREPARAMS(this, packet));

	Channel* channel = Channel::find(this->global.channels, packet.args[1]);
	if (!channel)
		throw Numeric(ERR_NOSUCHCHANNEL(this, packet.args[1]));
	if (!this->opped && !ft_vecexists(channel->operlist, this->nickname))
		throw Numeric(ERR_CHANOPRIVSNEEDED(this, channel));
	
	Client* client = Client::find(this->global.clients, packet.args[2]);
	if (!client)
		throw Numeric(ERR_NOSUCHNICK(this, packet.args[2]));
	if (!ft_vecexists(channel->clients, client))
		throw Numeric(ERR_USERNOTINCHANNEL(this, client, channel));

	ft_vecrem(channel->clients, client);
	ft_vecrem(client->channels, channel);
	this->write(KICK(this, client, channel));
	client->write(PART(client, channel, "Kicked"));
	return ;
}

void	Client::onListPacket(const t_packet& packet)
{
	if (packet.args.size() < 2)
	{
		this->write(RPL_LISTSTART(this));
		MChannels::iterator it = this->global.channels.begin();
		for (; it != this->global.channels.end(); it++)
			this->write(RPL_LIST(this, it->second));	
		this->write(RPL_LISTEND(this));
	}

	if (packet.args.size() == 3)
	{
		Channel* channel = Channel::find(this->global.channels, packet.args[1]);
		if (!channel)
			throw Numeric(ERR_NOSUCHCHANNEL(this, packet.args[1]));
		this->write(RPL_LISTSTART(this));
		this->write(RPL_LIST(this, channel));
		this->write(RPL_LISTEND(this));
	}
}

void	Client::onPartPacket(const t_packet& packet)
{
	if (packet.args.size() < 2)
		throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
	
	Channel* channel = Channel::find(this->global.channels, packet.args[1]);
	if (!channel)
		throw Numeric(ERR_NOSUCHCHANNEL(this, packet.args[1]));
	if (!ft_vecexists(channel->clients, this))
		throw Numeric(ERR_NOTONCHANNEL(this, channel));
	ft_vecrem(channel->clients, this);
	ft_vecrem(this->channels, channel);
	this->write(PART(this, channel, packet.rest));
}

void	Client::onModePacket(const t_packet& packet)
{
	if (packet.args.size() < 2)
		throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
	if (!Channel::isname(packet.args[1]))
		return ;

	Channel* channel = Channel::find(this->global.channels, packet.args[1]);
	if (!channel)
		throw Numeric(ERR_NOSUCHCHANNEL(this, packet.args[1]));
	if (!this->opped && !ft_vecexists(channel->operlist, this->nickname))
		throw Numeric(ERR_CHANOPRIVSNEEDED(this, channel));
	if (packet.args.size() == 2)
		return (this->write(RPL_CHANNELMODEIS(this, channel)));

	if (packet.args[2] == "+n")
	{
		channel->closed = true;
		return ;
	}
		
	if (packet.args[2] == "-n")
	{
		channel->closed = false;
		return ;
	}

	if (packet.args[2] == "+i")
	{
		channel->invite = true;
		channel->closed = true;
		return ;
	}

	if (packet.args[2] == "-i")
	{
		channel->invite = false;
		return ;
	}

	if (packet.args[2] == "+k")
	{
		if (packet.args.size() < 4)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		channel->password = packet.args[3];
		channel->closed = true;
		return ;
	}

	if (packet.args[2] == "-k")
	{
		channel->password.erase();
		return ;
	}

	if (packet.args[2] == "+b")
	{
		if (packet.args.size() == 3)
		{
			for (size_t i = 0; i < channel->banlist.size(); i++)
				this->write(RPL_BANLIST(this, channel->banlist[i], channel));
			this->write(RPL_ENDOFBANLIST(this, channel));
			return ;
		}
		if (packet.args.size() == 4)
		{
			if (ft_vecexists(channel->banlist, packet.args[3]))
				return ;
			channel->banlist.push_back(packet.args[3]);
			Client* target = Client::find(channel->clients, packet.args[3]);
			if (!target)
				return ;
			ft_vecrem(channel->clients, target);
			ft_vecrem(target->channels, channel);
			target->write(PART(target, channel, "Banned"));
			return ;
		}
	}
	
	if (packet.args[2] == "-b")
	{
		if (packet.args.size() < 4)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		if (!ft_vecexists(channel->banlist, packet.args[3]))
			return ;
		ft_vecrem(channel->banlist, packet.args[3]);
		return ;
	}

	if (packet.args[2] == "+o")
	{
		if (packet.args.size() < 4)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		if (ft_vecexists(channel->operlist, packet.args[3]))
			return ;
		channel->operlist.push_back(packet.args[3]);
		return ;
	}

	if (packet.args[2] == "-o")
	{
		if (packet.args.size() < 4)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		if (!ft_vecexists(channel->operlist, packet.args[3]))
			return ;
		ft_vecrem(channel->operlist, packet.args[3]);
		return ;
	}
}

void	Client::motd(void)
{
	ifstream file("motd.txt");
	if (!file)
		throw Numeric(ERR_NOMOTD(this));

	this->write(RPL_MOTDSTART(this, "Chadland"));
	for (string line; getline(file, line);)
		this->write(RPL_MOTD(this, line));
	this->write(RPL_ENDOFMOTD(this));
}

void	Client::onPacket(const t_packet& packet)
{
	cout << "<- " << packet.raw << endl;

	if (packet.args.size() == 0)
		return ;
	if (packet.args[0] == "motd")
		return (this->motd());
	if (packet.args[0] == "PING")
		return (this->onPingPacket(packet));

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
		this->state = REGISTERED;
		this->write(RPL_WELCOME(this, "Chadnet"));
		this->write(RPL_YOURHOST(this));
		this->motd();
		return ;
	}

	if (packet.args[0] == "QUIT")
		return (this->onQuitPacket(packet));
	if (packet.args[0] == "JOIN")
		return (this->onJoinPacket(packet));
	if (packet.args[0] == "NOTICE")
		return (this->onNoticePacket(packet));
	if (packet.args[0] == "NICK")
		return (this->onNickPacket(packet));
	if (packet.args[0] == "OPER")
		return (this->onOperPacket(packet));
	if (packet.args[0] == "MODE")
		return (this->onModePacket(packet));
	if (packet.args[0] == "KICK")
		return (this->onKickPacket(packet));
	if (packet.args[0] == "INVITE")
		return (this->onInvitePacket(packet));
	if (packet.args[0] == "PRIVMSG")
		return (this->onMessagePacket(packet));
	if (packet.args[0] == "LIST")
		return (this->onListPacket(packet));
	if (packet.args[0] == "PART")
		return (this->onPartPacket(packet));

	if (packet.args[0] == "TIME")
	{
		if (packet.args.size() < 2)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		this->write(this->nickname + " TIME " + "127.0.0.1");
		return ;
	}

	if (packet.args[0] == "INFO")
	{
		if (packet.args.size() < 2)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		this->write("371 :Hello world");
		this->write("374 :End of info");
		return ;
	}

	if (packet.args[0] == "ADMIN")
	{
		if (packet.args.size() < 2)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		this->write(":" + this->nickname + " ADMIN " + "127.0.0.1");
		return ;
	}

	if (packet.args[0] == "VERSION")
	{
		if (packet.args.size() < 2)
			throw Numeric(ERR_NEEDMOREPARAMS(this, packet));
		this->write(":" + this->nickname + " VERSION " + "127.0.0.1");
		return ;
	}

	throw Numeric(ERR_UNKNOWNCOMMAND(this, packet));
}