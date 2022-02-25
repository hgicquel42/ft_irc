#include "replies.hpp"

#include "utils/format.hpp"

string	PONG(const string& token)
{
	vector<string> args;
	args.push_back(token);
	return (ft_format("PONG %s", args));
}

string	ERROR(const string& reason)
{
	vector<string> args;
	args.push_back(reason);
	return (ft_format("ERROR :%s", args));
}

string	JOIN(const Client* client, const Channel* channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);
	return (ft_format(":%s@Chadnet JOIN %s", args));
}

string	QUIT(const Client* client, const string& reason)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(reason);
	return (ft_format(":%s QUIT :%s", args));
}

string	PRIVMSG(const Client* client, const string& target, const string& message)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(target);
	args.push_back(message);
	return (ft_format(":%s PRIVMSG %s :%s", args));
}

string	NOTICE(const Client* client, const string& target, const string& message)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(target);
	args.push_back(message);
	return (ft_format(":%s NOTICE %s :%s", args));
}

string	INVITE(const Client* client, const Client* target, const Channel* channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(target->nickname);
	args.push_back(channel->name);
	return (ft_format(":%s INVITE %s %s", args));
}

string	KICK(const Client* client, const Client* target, const Channel* channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);
	args.push_back(target->nickname);
	return (ft_format(":%s KICK %s %s", args));
}

string	NICK(const Client* client, const string& nickname)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(nickname);
	return (ft_format(":%s NICK %s", args));
}

string	PART(const Client* client, const Channel* channel, const string& reason)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);
	args.push_back(reason);
	return (ft_format(":%s@Chadnet PART %s :%s", args));
}

string	RPL_WELCOME(const Client* client, const string& network)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(network);
	args.push_back(client->nickname);
	return (ft_format("001 %s :Welcome to the %s Network bitches, %s", args));
}

string	RPL_YOURHOST(const Client* client)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(client->hostname);
	args.push_back("0.1 alpha-SNAPSHOT");
	return (ft_format("002 %s :Your host is %s, running version %s", args));
}

string	RPL_LISTSTART(const Client* client)
{
	vector<string> args;
	args.push_back(client->nickname);
	return (ft_format("321 %s Channel :Users  Name", args));
}

string	RPL_LIST(const Client* client, const Channel* channel)
{
	vector<string> args;
	stringstream stream;

	args.push_back(client->nickname);
	args.push_back(channel->name);
	stream << channel->clients.size();
	args.push_back(stream.str());
	return (ft_format("322 %s %s %s", args));
}

string	RPL_LISTEND(const Client* client)
{
	vector<string> args;
	args.push_back(client->nickname);
	return (ft_format("323 %s :End of /LIST", args));
}

string	RPL_CHANNELMODEIS(const Client* client, const Channel* channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);

	string mode = "+";
	if (channel->closed)
		mode += "n";
	if (!channel->password.empty())
		mode += "k";
	if (channel->invite)
		mode += "i";
	args.push_back(mode);
	args.push_back(string());
	return (ft_format("324 %s %s %s %s", args));
}

string	RPL_INVITING(const Client* client, const Client* target, const Channel* channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(target->nickname);
	args.push_back(channel->name);
	return (ft_format("341 %s %s %s", args));
}

string	RPL_BANLIST(const Client* client, const string& mask, const Channel* channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);
	args.push_back(mask);
	return (ft_format("367 %s %s %s", args));
}

string	RPL_ENDOFBANLIST(const Client* client, const Channel* channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);
	return (ft_format("368 %s %s :End of channel ban list", args));
}

string	RPL_MOTDSTART(const Client* client, const string& server)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(server);
	return (ft_format("375 %s :- %s Message of the day - ", args));
}

string	RPL_MOTD(const Client* client, const string& line)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(line);
	return (ft_format("372 %s :%s", args));
}

string	RPL_ENDOFMOTD(const Client* client)
{
	vector<string> args;
	args.push_back(client->nickname);
	return (ft_format("376 %s :End of /MOTD command.", args));
}

string	RPL_YOUREOPER(const Client* client)
{
	vector<string> args;
	args.push_back(client->nickname);
	return (ft_format("381 %s :You are now an IRC operator", args));
}