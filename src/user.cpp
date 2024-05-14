/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 12:01:47 by jariza-o          #+#    #+#             */
/*   Updated: 2024/05/14 15:01:45 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

User::User()
{
	
}
User::~User()
{
	
}
User::User(const User &u)
{
}
User	&User::operator=(const User &c) // Porque es private???
{
	
}
void	User::set_nickname(std::string nickname)
{
	
}
const std::string	User::get_nickname() const
{
	
}
void	User::set_username(std::string username)
{
	
}
const std::string	User::get_username() const
{
	
}
void	User::set_realname(std::string realname)
{
	
}
const std::string	User::get_realname() const
{
	
}
void	User::set_host(std::string host)
{
	
}
const std::string	User::get_host() const
{
	
}
void	User::set_server(std::string server)
{
	
}
const std::string	User::get_server() const
{
	
}
void	User::set_is_authenticated(bool is_authenticated)
{
	
}
const bool	User::get_is_authenticated() const
{
	
}
void	User::set_fd(int fd)
{
	
}
const int	User::get_fd() const
{
	
}
void	User::enqueue_message(std::string &message)
{
	
}
const std::string	User::dequeue_message()
{
	
}
const bool	User::has_queued_messages() const
{
	
}