#include "params.hpp"

#include "utils/numbers.hpp"
#include "utils/errors.hpp"

/**
 * @brief parse params
 * 
 * @param argc 
 * @param argv 
 * @return t_params 
 */
t_params	ft_params(int argc, char **argv)
{
	t_params	params;

	if (argc != 3)
		throw Exception("Invalid format: <port> <password>");
	if (!ft_xatoui16(argv[1], &params.port))
		throw Exception("Port must be between 0 and 65535");
	params.password = argv[2];
	if (params.password.length() < 3)
		throw Exception("Password must be at least 3 characters long");
	return (params);
}