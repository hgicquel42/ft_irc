#pragma once

#include "client.hpp"
#include "channel.hpp"

using namespace std;

string	RPL_WELCOME(const Client* client, const string& network);

string	RPL_YOURHOST(const Client* client);

string	RPL_MOTDSTART(const Client* client, const string& server);

string	RPL_MOTD(const Client* client, const string& line);

string	RPL_ENDOFMOTD(const Client* client);