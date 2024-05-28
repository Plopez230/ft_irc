/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:10:02 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/28 12:53:23 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

Bot::Bot(){}
Bot::Bot(const char* ipAddress, const char* port, const char* pass)
{
	this->ipAddress = ipAddress;
	this->port = port;
	this->pass = pass;

	this->h.ai_family = AF_INET;
	this->h.ai_socktype = SOCK_STREAM;
	this->h.ai_protocol = IPPROTO_TCP;
	this->h.ai_flags = AI_PASSIVE;

	this->addrinf = getaddrinfo(this->ipAddress.c_str(), this->port.c_str(), &this->h, &this->r);
	if (this->addrinf < 0)
	{
		throw std::runtime_error("getaddrinfo failed");
	}
	this->sock = socket(this->r->ai_family, this->r->ai_socktype, this->r->ai_protocol);
	if (this->sock < 0)
	{
		throw std::runtime_error("socket failed");
	}
	freeaddrinfo(this->r);
	if (connect(this->sock, this->r->ai_addr, this->r->ai_addrlen) == -1)
	{
		throw std::runtime_error("connection failed");
	}
	this->send("PASS " + this->pass);
}
Bot::~Bot()
{
}

void	Bot::send(const std::string& message)
{
	std::string mens = message + "\r\n";
	if (::send(this->sock, mens.c_str(), mens.length(), 0) == -1)
	{
		throw std::runtime_error("send failed");
	}
}

std::string	Bot::receive()
{
	char buffer[512];

	int bytes_received = recv(this->sock, buffer, sizeof buffer - 1, 0);
	if (bytes_received == -1) {
		throw std::runtime_error("recv failed");
	}
	buffer[bytes_received] = '\0';
	return std::string(buffer);
}

std::vector<std::string> split(
    const std::string &s, char del, bool include_delimiter)
{
    std::vector<std::string> result;
    unsigned int start = 0;
    unsigned int end = 0;

    while (start < s.size())
    {
        while (s[start] == del && start < s.size())
        {
            start ++;
        }
        
        end = start;

        if (start > 0 && include_delimiter)
        {
            start -= 1;
        }
        
        while (s[end] != del && end < s.size())
        {
            end ++;
        }
        
        if (start != end)
        {
            result.push_back(s.substr(start, end - start));
        }
        
        start = end + 1;
    }
    return result;
}
std::string	Bot::get_randomPhrase() const
{
	srand(time(NULL));
	int i = rand() % 6;
	switch (i)
	{
		case 0:
			return "Ser o no ser, esa es la cuestion";
		case 1:
			return "Todos los pajaros comen trigo y la culpa para el gorrion";
		case 2:
			return "Te digo un secreto??? Belen no es peliroja natural!!!!";
		case 3:
			return "Ni ChatGPT seria capaz de responder esto";
		case 4:
			return "Tagarninas con huevos";
		case 5:
			return "Sou un cupcake y QUEEEEEEEEEEEEEEEEEEEEE!!!!!!!";
	}
	
	return "jAJJAJAJAJAJJAjA";
}