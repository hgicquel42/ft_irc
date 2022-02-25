#include "main.hpp"

#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "utils/colors.hpp"
#include "utils/sockets.hpp"
#include "utils/format.hpp"

#include "global.hpp"
#include "params.hpp"
#include "poll.hpp"

using namespace std;

t_global global;

void	ft_signal(int signal)
{
	(void) signal;
	global.running = false;
}

void	ft_start(int argc, char **argv)
{
	global.running = true;
	global.params = ft_params(argc, argv);

	try {
		global.server = ft_slisten(global.params.port);
	} catch(exception& e) {
		throw Exception("Could not bind port");
	}

	cout << "Listening on port ";
	cout << global.params.port << "\n";

	ft_poll(global);
}

void	ft_exit(t_global& global)
{
	MChannels::iterator it = global.channels.begin();
	for (; it != global.channels.end(); it++)
		delete it->second;
	for (size_t i = 0; i < global.clients.size(); i++)
	{
		close(global.clients[i]->socket.file);
		delete global.clients[i];
	}
	close(global.server.file);
}

int	main(int argc, char **argv)
{
	try {
		signal(SIGINT, ft_signal);
		signal(SIGTERM, ft_signal);
		signal(SIGQUIT, ft_signal);
		ft_start(argc, argv);
		ft_exit(global);
		return (EXIT_SUCCESS);
	} catch(exception& e) {
		ft_exit(global);
		cerr << ft_red(e.what()) << "\n";
		return (EXIT_FAILURE);
	}
}