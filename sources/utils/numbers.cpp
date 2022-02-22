#include "numbers.hpp"

#include "chars.hpp"

/**
 * @brief parse 16-bits unsigned int
 * 
 * @param string 
 * @param result 
 * @return true 
 * @return false 
 */
bool	ft_xatoui16(std::string string, int *result)
{
	int		i = 0;
	int		x = 0;

	while (ft_isdigit(string[i]) && x < 65535)
		x = (x * 10) + (string[i++] - '0');	
	if (string[i] || x >= 65535)
		return (false);
	*result = x;
	return (true);
}
