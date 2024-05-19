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

User::User()
{
	this->is_registered = false;
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

void	User::set_pass(const std::string &pass)
{
	this->pass = pass;
}

const std::string	User::get_pass() const
{
	return this->pass;
}

void	User::set_is_registered(const bool is_authenticated)
{
	this->is_registered = is_authenticated;
}

bool	User::get_is_registered() const
{
	return this->is_registered;
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
	{
		throw std::runtime_error("The message queue is empty!");
	}

	std::string	message = this->output_queue.front();
	this->output_queue.pop();

	return message;
}

bool	User::has_queued_messages() const
{
	return this->output_queue.size() > 0;
}

std::vector<User *>::iterator find_user_by_nickname(std::vector<User *> &v,
	const std::string &nickname)
{
	std::vector<User *>::iterator	pos = v.begin();

	for (; pos != v.end(); pos++)
	{
		if (to_lower((*pos)->get_nickname())
			== to_lower(nickname))
		{
			return pos;
		}
	}
	return v.end();
}

std::vector<User *>::iterator find_user_by_fd(std::vector<User *> &v, int fd)
{
    for (std::vector<User *>::iterator u = v.begin();
        u != v.end(); u++)
    {
        if ((*u)->get_fd() == fd)
            return u;
    }

    return v.end();
}
