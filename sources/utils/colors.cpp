#include "colors.hpp"

using namespace std;

/**
 * @brief colorize text
 * 
 * @param text 
 * @return string 
 */
string	ft_red(string text)
{
	return (CRED + text + CRESET);
}

/**
 * @brief colorize text
 * 
 * @param text 
 * @return string 
 */
string	ft_green(string text)
{
	return (CGREEN + text + CRESET);
}
