#pragma once

#include "client.hpp"
#include "channel.hpp"
#include <sstream>

using namespace std;

string	PONG(const string& token);

string	ERROR(const string& reason);

string	JOIN(const Client* client, const Channel* channel);

string	QUIT(const Client* client, const string& reason);

string	PRIVMSG(const Client* client, const string& target, const string& message);

string	NOTICE(const Client* client, const string& target, const string& message);

string	INVITE(const Client* client, const Client* target, const Channel* channel);

string	KICK(const Client* client, const Client* target, const Channel* channel);

string	NICK(const Client* client, const string& nickname);

string	PART(const Client* client, const Channel* channel, const string& reason);

string	RPL_WELCOME(const Client* client, const string& network);

string	RPL_YOURHOST(const Client* client);

string	RPL_LISTSTART(const Client* client);

string	RPL_LIST(const Client* client, const Channel* channel);

string	RPL_LISTEND(const Client* client);

string	RPL_CHANNELMODEIS(const Client* client, const Channel* channel);

string	RPL_INVITING(const Client* client, const Client* target, const Channel* channel);

string	RPL_BANLIST(const Client* client, const string& mask, const Channel* channel);

string	RPL_ENDOFBANLIST(const Client* client, const Channel* channel);

string	RPL_MOTDSTART(const Client* client, const string& server);

string	RPL_MOTD(const Client* client, const string& line);

string	RPL_ENDOFMOTD(const Client* client);

string	RPL_YOUREOPER(const Client* client);