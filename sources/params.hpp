#pragma once

#include <string>

typedef struct s_params
{
	int			port;
	std::string	password;
}	t_params;

/**
 * @brief parse params
 * 
 * @param argc 
 * @param argv 
 * @return t_params 
 */
t_params	ft_params(int argc, char **argv);