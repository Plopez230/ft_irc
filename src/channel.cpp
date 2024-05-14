/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:47:57 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/14 16:28:27 by jariza-o         ###   ########.fr       */
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

void Channel::set_topic(const std::string topic)
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

const bool Channel::has_mode(int mode) const
{
	return (this->mode & mode) == mode;
}

void Channel::set_operator(User *user)
{
	this->operators.push_back(user);
}

void Channel::remove_operator(const std::string &nickname)
{
	if (this->is_operator(nickname))
		this->operators.erase(this->get_operator(nickname));
}

std::vector<User *>::iterator Channel::get_operator(const std::string &nickname) const
{
	return get_user_by_nickname(this->operators, nickname);
}

const bool Channel::is_operator(const std::string &nickname) const
{
	return get_user_by_nickname(this->operators, nickname) != this->operators.end();
}

void Channel::set_user(User *user)
{
	this->users.push_back(user);
}

void Channel::remove_user(const std::string &nickname)
{
	if (this->is_user(nickname))
		this->users.erase(this->get_operator(nickname));
}

std::vector<User *>::iterator Channel::get_user(const std::string &nickname) const
{
	return get_user_by_nickname(this->users, nickname);
}

const bool Channel::is_user(const std::string &nickname) const
{
	return get_user_by_nickname(this->users, nickname) != this->users.end();
}

void Channel::set_invitation(User *user)
{
	this->invitations.push_back(user);
}

void Channel::remove_invitation(const std::string &nickname)
{
	if (this->is_invitation(nickname))
		this->invitations.erase(this->get_operator(nickname));
}

std::vector<User *>::iterator Channel::get_invitation(const std::string &nickname) const
{
	return get_user_by_nickname(this->invitations, nickname);
}

const bool Channel::is_invitation(const std::string &nickname) const
{
	return get_user_by_nickname(this->invitations, nickname) != this->invitations.end();
}
