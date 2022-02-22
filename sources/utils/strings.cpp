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
 * @brief true if string is all spaces
 * 
 * @param s 
 * @return true 
 * @return false 
 */
bool	ft_strisspace(const std::string s)
{
	for (size_t i = 0; i < s.length(); i++)
		if (!std::isspace(s[i]))
			return (false);
	return (true);
}

/**
 * @brief true if string has at least one space
 * 
 * @param s 
 * @return true 
 * @return false 
 */
bool	ft_strhasspace(const std::string s)
{
	for (size_t i = 0; i < s.length(); i++)
		if (std::isspace(s[i]))
			return (true);
	return (false);
}

/**
 * @brief split by crlf
 * 
 * @param text 
 * @param c 
 * @return std::vector<std::string> 
 */
std::vector<std::string>	ft_split(std::string text)
{
	std::stringstream			stream(text);
	std::vector<std::string>	result;
	std::string					token;

	while (std::getline(stream, token))
	{
		if (*token.rbegin() == '\r' && token.size() == 1)
			continue ;
		if (*token.rbegin() == '\r')
			token = token.substr(0, token.size() - 1);
		result.push_back(token);
	}
	return (result);
}

/**
 * @brief split by char
 * 
 * @param text 
 * @param c 
 * @return std::vector<std::string> 
 */
std::vector<std::string>	ft_splitby(std::string text, char c)
{
	std::stringstream			stream(text);
	std::vector<std::string>	result;
	std::string					token;

	while (std::getline(stream, token, c))
		result.push_back(token);
	return (result);
}