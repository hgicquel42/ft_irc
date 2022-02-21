#include "strings.hpp"

/**
 * @brief remove last character
 * 
 * @param s 
 * @return std::string 
 */
std::string ft_trimend(std::string s)
{
	return (s.substr(0, s.size() - 1));
}
