#include "strings.hpp"

#include <sstream>

/**
 * @brief remove last character
 * 
 * @param s 
 * @return std::string 
 */
std::string ft_strpop(const std::string s)
{
	return (s.substr(0, s.size() - 1));
}

/**
 * @brief split by char
 * 
 * @param text 
 * @param c 
 * @return std::vector<std::string> 
 */
std::vector<std::string>	ft_split(std::string text, char c)
{
	std::stringstream			stream(text);
	std::vector<std::string>	result;
	std::string					token;

	while (std::getline(stream, token, c))
		result.push_back(token);
	return (result);
}