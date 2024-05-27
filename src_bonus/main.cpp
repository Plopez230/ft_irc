/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:04:36 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/27 19:06:53 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"
#include <iostream>
#include <unistd.h>

int	main(int argc, char** argv)
{
	if (argc < 4)
	{
		std::cout << "ERROR!! The parameters number is incorrect:" << std::endl;
		std::cout << "./bot <IP Server> <Port Number> <Password>" << std::endl;
		return 1;
	}
	try 
	{
		Bot	server(argv[1], argv[2], argv[3]);
		server.send("NICK emilio");
		//std::cout << server.receive() << std::endl;
		server.send("USER a a a a");
		//std::cout << server.receive() << std::endl;
		while (1)
		{
			std::string line;
			std::cin >> line;
			std::cout << line << "hjkhjk" << std::endl;
			server.send(line);
			//server.receive();
		}
	}
	catch(const std::runtime_error& e)
	{
		std::cout << e.what() << std::endl;
	}
}
