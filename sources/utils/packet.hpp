#pragma once

#include <vector>
#include <string>

using namespace std;

typedef struct s_packet
{
	string			raw;
	vector<string>	args;
	string			rest;
}	t_packet;

/**
 * @brief unpack raw packet
 * 
 * @param raw 
 * @return t_packet 
 */
t_packet	ft_unpack(const string& raw);