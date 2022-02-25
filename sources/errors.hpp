#pragma once

#include "client.hpp"
#include "channel.hpp"

using namespace std;

string	ERR_NOSUCHNICK(const Client* client, const string& nickname);

string	ERR_NOSUCHCHANNEL(const Client* client, const string& channel);

string	ERR_CANNOTSENDTOCHAN(const Client* client, const Channel *channel);

string	ERR_UNKNOWNCOMMAND(const Client* client, const t_packet& packet);

string  ERR_NOMOTD(const Client* client);

string	ERR_NICKNAMEINUSE(const Client* client, const string& nickname);

string	ERR_USERNOTINCHANNEL(const Client* client, const Client* target, const Channel *channel);

string	ERR_NOTONCHANNEL(const Client* client, const Channel *channel);

string	ERR_USERONCHANNEL(const Client* client, const Client* target, const Channel *channel);

string	ERR_NEEDMOREPARAMS(const Client* client, const t_packet& packet);

string	ERR_PASSWDMISMATCH(const Client* client);

string	ERR_INVITEONLYCHAN(const Client* client, const Channel *channel);

string	ERR_BANNEDFROMCHAN(const Client* client, const Channel *channel);

string	ERR_BADCHANNELKEY(const Client* client, const Channel *channel);

string	ERR_CHANOPRIVSNEEDED(const Client* client, const Channel *channel);
