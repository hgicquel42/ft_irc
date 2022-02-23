#pragma once

#include <string>
#include <vector>

typedef struct s_global t_global;

#include "global.hpp"
#include "client.hpp"

using namespace std;

class Channel
{
	private:

	public:
		string	name;
		string	pass;
		bool	inviteonly;
		bool	passonly;

		vector<Client*>	clients;
		
		Channel(const string& name);
		Channel(const Channel& from);
		~Channel(void);

		Channel&	operator=(const Channel& from);

		static Channel* find(t_global& global, const string& name);

		static Channel*	findOrCreate(t_global& global, const string& name);
};