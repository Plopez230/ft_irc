/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:35:18 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/28 15:41:47 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "irc.hpp"
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <ctime>
#include <cstdlib>

class Bot
{
public:
	Bot();
	Bot(const char* ipAddress, const char* port, const char* pass, const char* nick);
	~Bot();

	void		send(const std::string& message);
	std::string	receive();
	std::string	get_randomPhrase() const;
	std::string	get_nick() const;

private:
	Bot(const Bot& src);
	Bot& 						operator=(const Bot& src);
	int							sock;
	int							addrinf;
	std::string					ipAddress, port, pass, nick;
	addrinfo					h;
	addrinfo*					r;
	std::vector<std::string>	phrases;
	
};

std::vector<std::string> split(const std::string &s, char del, bool include_delimiter);

#endif