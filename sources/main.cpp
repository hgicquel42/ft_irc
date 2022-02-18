#include "main.hpp"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>

#include "utils/colors.hpp"
#include "utils/numbers.hpp"
#include "utils/sockets.hpp"
#include "error.h"

int	ft_error(std::string s)
{
	std::cerr << ft_red(s) << "\n";
	return (EXIT_FAILURE);
}

t_params	ft_params(int argc, char **argv)
{
	t_params				params;

	if (argc != 3)
		throw Exception("Invalid format: <port> <password>");
	if (!ft_xatoui16(argv[1], &params.port))
		throw Exception("Port must be between 0 and 65535");
	params.password = argv[2];
	if (params.password.length() < 3)
		throw Exception("Password must be at least 3 characters long");
	return (params);
}

int	main(int argc, char **argv)
{
	t_params				params;
	t_socket				server;
	t_socket				client;
	std::vector<t_socket>	clients;

	try {
		params = ft_params(argc, argv);
	} catch(std::exception& e) {
		return (ft_error(e.what()));
	}
	
	try {
		server = ft_listen(params.port);
	} catch(std::exception& e) {
		return (ft_error("Couldn't bind port"));
	}

	std::cout << "Listening on port " << params.port << "\n";

	while (true)
	{
		client = ft_accept(server);
		clients.push_back(client);
	}

	return (EXIT_SUCCESS);
}