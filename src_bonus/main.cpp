/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:04:36 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/28 16:31:50 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"
#include <csignal>

static void signal_handler(int signal)
{
	(void) signal;
	throw std::runtime_error("Stopping...");
}

int	main(int argc, char** argv)
{
	if (argc < 5)
	{
		std::cout << "ERROR!! The parameters number is incorrect:" << std::endl;
		std::cout << "./bot <IP Server> <Port Number> <Password> <NickName>" << std::endl;
		return 1;
	}
	try 
	{
		std::signal(SIGINT, &signal_handler);
		std::signal(SIGTERM, &signal_handler);

		Bot	server(argv[1], argv[2], argv[3], argv[4]);
		server.send("NICK " + server.get_nick());
		server.send("USER a a a a");
		while (1)
		{
			std::string	mess = server.receive();
			std::cout << mess << std::endl;
			std::vector<std::string> mess_splitted = ::split(mess, ' ', false);
			if (mess_splitted[1] == "INVITE")
			{
				server.send("JOIN " + mess_splitted[3]);
				sleep(2);
				mess_splitted[3].erase((mess_splitted[3].length() - 1), mess_splitted[3].length());
				std::string m = "PRIVMSG " + mess_splitted[3] + " :" + "Hola, soy " + server.get_nick() + ", el bot mas loco. Te respondere cosas sin sentido: AGUACATE!!!!";
				server.send(m);
			}
			else if (mess_splitted[1] == "PRIVMSG")
			{
				server.send("PRIVMSG " + mess_splitted[2] + " :" + server.get_randomPhrase());
			}
		}
	}
	catch(const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
}
