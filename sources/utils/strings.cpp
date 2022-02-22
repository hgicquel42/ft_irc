#include "strings.hpp"

#include <sstream>

#include <iostream>

/**
 * @brief remove last character
 * 
 * @param s 
 * @return std::string 
 */
std::string ft_strpop(const std::string s)
{
	std::cout << s.size() << std::endl;
	return (s.substr(0, s.size() - 1));
}

/**
 * @brief true if a starts with b
 * 
 * @param a 
 * @param b 
 * @return bool 
 */
bool	ft_strstarts(const std::string a, const std::string b)
{
	return (a.rfind(b, 0) == 0);
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