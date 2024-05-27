/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:35:18 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/27 19:02:06 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "irc.hpp"
#include <iostream>

class Bot
{
public:
	Bot();
	Bot(const char* ipAddress, const char* port, const char* pass);
	~Bot();

	void		send(const std::string& message);
	std::string	receive();

private:
	Bot(const Bot& src);
	Bot& 		operator=(const Bot& src);
	int			sock;
	std::string	ipAddress, port, pass;
	addrinfo	h;
	addrinfo*	r;
	
};



#endif