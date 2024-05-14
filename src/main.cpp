/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 01:32:55 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/14 12:05:23 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <iostream>
#include <csignal>

static bool running = true;

static void signal_handler(int signal)
{
	running = false;
}

int main (int argc, char **argv)
{
	try
	{
		if (argc != 3)
			throw std::runtime_error("gooby pls");
		std::signal(SIGINT, &signal_handler);
		std::signal(SIGTERM, &signal_handler);
		Server server(argv[2]);
		SocketManager socket_manager(get_port_number(argv[1]), server);
		while (running)
			socket_manager.loop();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
