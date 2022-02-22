#pragma once

#include <vector>
#include <string>

typedef struct s_packet
{
	std::string					raw;
	std::vector<std::string>	args;
	std::string					rest;
}	t_packet;

/**
 * @brief unpack raw packet
 * 
 * @param raw 
 * @return t_packet 
 */
t_packet	ft_unpack(const std::string& raw);