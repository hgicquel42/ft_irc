#include "packet.hpp"

#include <sstream>
#include <iostream>

#include "strings.hpp"

using namespace std;

/**
 * @brief unpack raw packet 
 * <some> <args> :<some rest>
 * 
 * @param raw 
 * @return t_packet 
 */
t_packet	ft_unpack(const string& raw)
{
	t_packet	packet;

	packet.raw = raw;
	size_t end = raw.find(':');
	if (end == string::npos)
		end = raw.size();
	packet.args = ft_splitby(raw.substr(0, end), ' ');
	if (end != raw.size())
		packet.rest = raw.substr(end + 1, raw.size() - end);
	return (packet);
}