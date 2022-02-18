#include "colors.hpp"

/**
 * @brief colorize text
 * 
 * @param text 
 * @return std::string 
 */
std::string	ft_red(std::string text)
{
	return (CRED + text + CRESET);
}

/**
 * @brief colorize text
 * 
 * @param text 
 * @return std::string 
 */
std::string	ft_green(std::string text)
{
	return (CGREEN + text + CRESET);
}
