#include "main.hpp"

#include <iostream>
#include <unistd.h>

#include "utils/colors.hpp"
#include "utils/sockets.hpp"

#include "global.hpp"
#include "params.hpp"
#include "poll.hpp"

void	ft_start(int argc, char **argv)
{
	t_global	global;

	global.params = ft_params(argc, argv);

	try {
		global.server = ft_slisten(global.params.port);
	} catch(std::exception& e) {
		throw Exception("Could not bind port\n");
	}

	std::cout << "Listening on port " << global.params.port << "\n";

	ft_poll(global);

	close(global.server.socket);
}

int	main(int argc, char **argv)
{
	try {
		ft_start(argc, argv);
		return (EXIT_SUCCESS);
	} catch(std::exception& e) {
		std::cerr << ft_red(e.what()) << "\n";
		return (EXIT_FAILURE);
	}
}