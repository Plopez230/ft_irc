/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:47:57 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/14 22:10:30 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

Channel::Channel()
{
	this->mode = 0;
}

Channel::~Channel()
{
	
}

void Channel::set_pass(const std::string &pass)
{
	this->pass = pass;
}

const std::string Channel::get_pass() const
{
	return this->pass;
}

void Channel::set_max_users(size_t max_users)
{
	this->max_users = max_users;
}

size_t Channel::get_max_users() const
{
	return this->max_users;
}

bool Channel::is_full() const
{
	return this->users.size() >= this->max_users;
}

void Channel::set_name(const std::string &name)
{
	this->name = name;
}

const std::string Channel::get_name() const
{
	return this->name;
}

void Channel::set_topic(const std::string &topic)
{
	this->topic = topic;
}

const std::string Channel::get_topic() const
{
	return this->topic;
}

void Channel::set_mode(int mode)
{
	this->mode = this->mode | mode;
}

void Channel::unset_mode(int mode)
{
	this->mode -= this->mode & mode;
}

bool Channel::has_mode(int mode) const
{
	return (this->mode & mode) == mode;
}

void Channel::add_operator(User *user)
{
	this->operators.push_back(user);
}

void Channel::remove_operator(const std::string &nickname)
{
	if (this->is_operator(nickname))
		this->operators.erase(this->find_operator(nickname));
}

std::vector<User *>::iterator Channel::find_operator(const std::string &nickname)
{
	return find_user_by_nickname(this->operators, nickname);
}

bool Channel::is_operator(const std::string &nickname)
{
	return find_user_by_nickname(this->operators, nickname) != this->operators.end();
}

void Channel::add_user(User *user)
{
	this->users.push_back(user);
}

void Channel::remove_user(const std::string &nickname)
{
	if (this->is_user(nickname))
		this->users.erase(this->find_operator(nickname));
}

std::vector<User *>::iterator Channel::find_user(const std::string &nickname)
{
	return find_user_by_nickname(this->users, nickname);
}

bool Channel::is_user(const std::string &nickname)
{
	return find_user_by_nickname(this->users, nickname) != this->users.end();
}

void Channel::add_invitation(User *user)
{
	this->invitations.push_back(user);
}

void Channel::remove_invitation(const std::string &nickname)
{
	if (this->is_invitation(nickname))
		this->invitations.erase(this->find_operator(nickname));
}

std::vector<User *>::iterator Channel::find_invitation(const std::string &nickname)
{
	return find_user_by_nickname(this->invitations, nickname);
}

bool Channel::is_invitation(const std::string &nickname)
{
	return find_user_by_nickname(this->invitations, nickname) != this->invitations.end();
}

std::vector<Channel *>::iterator find_channel_by_topic(std::vector<Channel *> &c, const std::string &topic)
{
	std::vector<Channel *>::iterator pos = c.begin();
	for (; pos != c.end(); pos++)
	{
		if ((*pos)->get_topic() == topic
			|| (*pos)->get_name() == topic)
			return pos;
	}
	return pos;
}

std::string Channel::get_nicknames()
{
	std::string names;
	for (size_t i = 0; i < this->users.size(); i++)
	{
		if (i > 0)
			names += " ";
		std::string nickname = this->users[i]->get_nickname();
		if (this->is_operator(nickname))
			names += "@";
		names += nickname;
	}
	return names;
}

void Channel::enqueue_message(const std::string &message)
{
	for (size_t i = 0; i < this->users.size(); i++)
		this->users[i]->enqueue_message(message);
}

void Channel::enqueue_message(const std::string &message, User *except)
{
	for (size_t i = 0; i < this->users.size(); i++)
	{
		if (except != this->users[i])
		{
			this->users[i]->enqueue_message(message);
		}
	}
}
