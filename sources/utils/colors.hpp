#pragma once

#include <string>

#define CRED "\033[0;31m"
#define CGREEN "\033[0;32m"
#define CRESET "\033[0m"

/**
 * @brief colorize text
 * 
 * @param text 
 * @return std::string 
 */
std::string	ft_red(std::string text);

/**
 * @brief colorize text
 * 
 * @param text 
 * @return std::string 
 */
std::string	ft_green(std::string text);