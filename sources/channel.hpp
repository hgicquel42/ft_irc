#pragma once

#include <string>
#include <vector>

typedef struct s_global t_global;

#include "global.hpp"
#include "client.hpp"

using namespace std;

typedef vector<Client*>			VClients;
typedef vector<Channel*>		VChannels;
typedef map<string, Channel*>	MChannels;

class Channel
{
	private:

	public:
		string	name;
		string	password;
		bool	invite;
		bool	closed;

		vector<Client*>	clients;
		vector<string>	invites;
		vector<string>	banlist;
		vector<string>	operlist;
		
		Channel(const string& name);
		Channel(const Channel& from);
		~Channel(void);

		Channel&	operator=(const Channel& from);

		static Channel* find(MChannels& channels, const string& name);

		static Channel*	findOrCreate(MChannels& channels, const string& name);

		static bool		isname(const string& name);
};