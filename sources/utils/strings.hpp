#pragma once

#include <string>
#include <vector>

using namespace std;

/**
 * @brief remove last character
 * 
 * @param s 
 * @return string 
 */
string ft_strpop(string s);

/**
 * @brief true if string is all spaces
 * 
 * @param s 
 * @return true 
 * @return false 
 */
bool	ft_strisspace(const string s);

/**
 * @brief true if string has at least one space
 * 
 * @param s 
 * @return true 
 * @return false 
 */
bool	ft_strhasspace(const string s);

/**
 * @brief split by crlf
 * 
 * @param text 
 * @param c 
 * @return vector<string> 
 */
vector<string>	ft_split(string text);

/**
 * @brief split by char
 * 
 * @param text 
 * @param c 
 * @return vector<string> 
 */
vector<string>	ft_splitby(string text, char c);