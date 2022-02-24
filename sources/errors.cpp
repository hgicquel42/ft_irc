#include "errors.hpp"

#include "utils/format.hpp"

#include "client.hpp"
#include "channel.hpp"

using namespace std;

string	ERR_NOSUCHNICK(const Client* client, const string& nickname)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(nickname);
	args.push_back("No such nick");
	return (ft_format("401 %s %s :%s", args));
}

string	ERR_NOSUCHCHANNEL(const Client* client, const string& channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel);
	args.push_back("No such channel");
	return (ft_format("403 %s %s :%s", args));
}

string	ERR_CANNOTSENDTOCHAN(const Client* client, const Channel *channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);
	args.push_back("Cannot send to channel");
	return (ft_format("404 %s %s :%s", args));
}

string	ERR_UNKNOWNCOMMAND(const Client* client, const t_packet& packet)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(packet.args[0]);
	args.push_back("Unknown command");
	return (ft_format("421 %s %s :%s", args));
}

string	ERR_NICKNAMEINUSE(const Client* client, const string& nickname)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(nickname);
	args.push_back("Nickname is already in use");
	return (ft_format("433 %s %s :%s", args));
}

string	ERR_USERNOTINCHANNEL(const Client* client, const Client* target, const Channel *channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(target->nickname);
	args.push_back(channel->name);
	args.push_back("They aren't on that channel");
	return (ft_format("441 %s %s %s :%s", args));
}

string	ERR_NOTONCHANNEL(const Client* client)
{
	
}

string	ERR_NEEDMOREPARAMS(const Client* client, const t_packet& packet)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(packet.args[0]);
	args.push_back("Not enough parameters");
	return (ft_format("461 %s %s :%s", args));
}

string	ERR_PASSWDMISMATCH(const Client* client)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back("Password incorrect");
	return (ft_format("464 %s :%s", args));
}

string	ERR_INVITEONLYCHAN(const Client* client, const Channel *channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);
	args.push_back("Cannot join channel (+i)");
	return (ft_format("473 %s %s :%s", args));
}

string	ERR_BADCHANNELKEY(const Client* client, const Channel *channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);
	args.push_back("Cannot join channel (+k)");
	return (ft_format("475 %s %s :%s", args));
}

string	ERR_CHANOPRIVSNEEDED(const Client* client, const Channel *channel)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(channel->name);
	args.push_back("You're not channel operator");
	return (ft_format("482 %s %s :%s", args));
}
