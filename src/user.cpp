/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:01:47 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/14 22:17:13 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <stdexcept> //ELIMINAR Y PONER EN EL .HPP

User::User()
{
	// ASIGNAR LOS STRINGS COMO CADENAS VACIAS
}

User::~User()
{}

void	User::set_nickname(const std::string &nickname)
{
	this->nickname = nickname;
}

const std::string	User::get_nickname() const
{
	return this->nickname;
}

void	User::set_username(const std::string &username)
{
	this->username = username;
}

const std::string	User::get_username() const
{
	return this->username;	
}
void	User::set_realname(const std::string &realname)
{
	this->realname = realname;
}

const std::string	User::get_realname() const
{
	return this->realname;
}

void	User::set_host(const std::string &host)
{
	this->host = host;
}

const std::string	User::get_host() const
{
	return this->host;
}

void	User::set_server(const std::string &server)
{
	this->server = server;
}

const std::string	User::get_server() const
{
	return this->server;
}

void	User::set_is_authenticated(const bool is_authenticated)
{
	this->is_authenticated = is_authenticated;
}

bool	User::get_is_authenticated() const
{
	return this->is_authenticated;
}

void	User::set_input_buffer(const std::string &input_buffer)
{
	this->input_buffer = input_buffer;
}

const std::string	User::get_input_buffer() const
{
	return this->input_buffer;
}

void	User::set_fd(int fd)
{
	this->fd = fd;
}

int	User::get_fd() const
{
	return this->fd;
}

void	User::enqueue_message(const std::string &message)
{
	this->output_queue.push(message);
}

const std::string	User::dequeue_message()
{
	if (this->output_queue.size() <= 0)
		throw std::runtime_error("The message queue is empty!");
	std::string	message = this->output_queue.front();
	this->output_queue.pop();
	return message;
}

bool	User::has_queued_messages() const
{
	if (this->output_queue.size() > 0)
		return true;
	return false;
}

std::vector<User *>::iterator	get_user_by_nickname(std::vector<User *> &v, const std::string &nickname)
{
	std::vector<User *>::iterator	pos = v.begin();
	for (; pos != v.end(); pos++)
	{
		if ((*pos)->get_nickname() == nickname)
			return pos;
	}
	return pos;
}
