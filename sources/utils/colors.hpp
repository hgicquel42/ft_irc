#pragma once

#include <string>

#define CRED "\033[0;31m"
#define CGREEN "\033[0;32m"
#define CRESET "\033[0m"

using namespace std;

/**
 * @brief colorize text
 * 
 * @param text 
 * @return string 
 */
string	ft_red(string text);

/**
 * @brief colorize text
 * 
 * @param text 
 * @return string 
 */
string	ft_green(string text);