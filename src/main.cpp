/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 17:23:27 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/14 17:23:27 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <iostream>
#include <fstream>
#include <csignal>
#include <cstdio>

static void signal_handler(int signal)
{
	(void) signal;
	throw StopServer("Stopping...");
}

static int get_port_number(char *arg)
{
    int port_number;
    char left;
    if (sscanf(arg, " %u %c", &port_number, &left) != 1)
        throw std::runtime_error("Not a port number");
    if (port_number < 1024)
        throw std::runtime_error("Reserved port");
    if (port_number < 1 || port_number > 65535)
        throw std::runtime_error("Port out of range");
    return port_number;
}

static void print_file(const std::string &filename)
{
	std::ifstream file(filename);
	std::cout << file.rdbuf();
	file.close();
}

int main (int argc, char **argv)
{
	try
	{
		if (argc != 3)
			throw std::runtime_error("gooby pls");
		print_file("./assets/logo.txt");
		std::signal(SIGINT, &signal_handler);
		std::signal(SIGTERM, &signal_handler);
		Server server(argv[2]);
		SocketManager socket_manager(get_port_number(argv[1]), server);
		while (true)
		{
			try
			{
				socket_manager.loop();
			}
			catch(const StopServer& e)
			{
				server.print_server_status(
					std::string("\033[1;31m") + e.what());
				break;
			}
			catch(const std::runtime_error& e)
			{
				server.print_server_status(
					std::string("\033[1;31m") + e.what());
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	return 0;
}
