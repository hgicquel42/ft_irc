#include "strings.hpp"

#include <sstream>

#include <iostream>

using namespace std;

/**
 * @brief remove last character
 * 
 * @param s 
 * @return string 
 */
string ft_strpop(const string s)
{
	return (s.substr(0, s.size() - 1));
}

/**
 * @brief true if string is all spaces
 * 
 * @param s 
 * @return true 
 * @return false 
 */
bool	ft_strisspace(const string s)
{
	for (size_t i = 0; i < s.length(); i++)
		if (!isspace(s[i]))
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
bool	ft_strhasspace(const string s)
{
	for (size_t i = 0; i < s.length(); i++)
		if (isspace(s[i]))
			return (true);
	return (false);
}

/**
 * @brief split by crlf
 * 
 * @param text 
 * @param c 
 * @return vector<string> 
 */
vector<string>	ft_split(string text)
{
	stringstream	stream(text);
	vector<string>	result;
	string			token;

	while (getline(stream, token))
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
 * @return vector<string> 
 */
vector<string>	ft_splitby(string text, char c)
{
	stringstream	stream(text);
	vector<string>	result;
	string			token;

	while (getline(stream, token, c))
		result.push_back(token);
	return (result);
}