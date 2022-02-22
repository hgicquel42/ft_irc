#pragma once

#include <string>
#include <vector>

/**
 * @brief remove last character
 * 
 * @param s 
 * @return std::string 
 */
std::string ft_strpop(std::string s);

/**
 * @brief true if string is all spaces
 * 
 * @param s 
 * @return true 
 * @return false 
 */
bool	ft_strisspace(const std::string s);

/**
 * @brief true if string has at least one space
 * 
 * @param s 
 * @return true 
 * @return false 
 */
bool	ft_strhasspace(const std::string s);

/**
 * @brief split by crlf
 * 
 * @param text 
 * @param c 
 * @return std::vector<std::string> 
 */
std::vector<std::string>	ft_split(std::string text);

/**
 * @brief split by char
 * 
 * @param text 
 * @param c 
 * @return std::vector<std::string> 
 */
std::vector<std::string>	ft_splitby(std::string text, char c);