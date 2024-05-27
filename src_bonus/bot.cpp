/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:10:02 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/27 19:06:39 by jariza-o         ###   ########.fr       */
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

	if (getaddrinfo(this->ipAddress.c_str(), this->port.c_str(), &this->h, &this->r) < 0)
	{
		throw std::runtime_error("getaddrinfo failed");
	}
	this->sock = socket(this->r->ai_family, this->r->ai_socktype, this->r->ai_protocol);
	if (this->sock < 0)
	{
		throw std::runtime_error("socket failed");
	}
	if (connect(this->sock, this->r->ai_addr, this->r->ai_addrlen) == -1)
	{
		throw std::runtime_error("connection failed");
	}
}
Bot::~Bot(){}

void	Bot::send(const std::string& message)
{
	std::string mens = message + "\r\n";
		std::cout << "TEST" << std::endl;
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