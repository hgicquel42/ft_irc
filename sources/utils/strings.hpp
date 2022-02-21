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
 * @brief split by char
 * 
 * @param text 
 * @param c 
 * @return std::vector<std::string> 
 */
std::vector<std::string>	ft_split(std::string text, char c);