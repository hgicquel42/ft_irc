#include "packet.hpp"

#include <sstream>

#include "strings.hpp"

/**
 * @brief unpack raw packet 
 * <some> <args> :<some rest>
 * 
 * @param raw 
 * @return t_packet 
 */
t_packet	ft_unpack(const std::string& raw)
{
	t_packet			packet;
	std::stringstream	stream(raw);
	
	std::string	rawargs;
	std::getline(stream, rawargs, ':');

	packet.raw = raw;
	packet.args = ft_splitby(rawargs, ' ');
	packet.rest = stream.str();
	return (packet);
}