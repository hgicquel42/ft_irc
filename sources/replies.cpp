#include "replies.hpp"

#include "utils/format.hpp"

string	RPL_WELCOME(const Client* client, const string& network)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(network);
	args.push_back(client->nickname);
	return (ft_format("001 %s :Welcome to the %s Network, %s", args));
}

string	RPL_YOURHOST(const Client* client)
{
	vector<string> args;
	args.push_back(client->nickname);
	args.push_back(client->hostname);
	args.push_back("0.1 alpha-SNAPSHOT");
	return (ft_format("002 %s :Your host is %s, running version %s", args));
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