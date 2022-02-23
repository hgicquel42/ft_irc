#pragma once

#include <string>

using namespace std;

typedef struct s_params
{
	int			port;
	string	password;
}	t_params;

/**
 * @brief parse params
 * 
 * @param argc 
 * @param argv 
 * @return t_params 
 */
t_params	ft_params(int argc, char **argv);