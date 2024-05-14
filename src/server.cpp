/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:47:21 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/14 16:19:41 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <iostream>

Server::Server(const std::string &pass)
{
    this->pass = pass;
    this->server_name = "irc.free_shipping.net";
    this->server_version = "v1.0";
    this->socket_manager = NULL;
}

Server::~Server()
{
    for (std::vector<User *>::iterator u = this->registered.begin();
        u != this->registered.end(); u++)
        delete *u;
    for (std::vector<Channel *>::iterator c = this->channels.begin();
        c != this->channels.end(); c++)
        delete *c;
}

const std::string Server::get_server_name() const
{
    return this->server_name;
}

const std::string Server::get_server_version() const
{
    return this->server_version;
}

const std::string Server::get_pass() const
{
    return this->pass;
}

void Server::set_socket_manager(SocketManager *socket_manager)
{
    this->socket_manager = socket_manager;
}

void Server::set_registered(User *user)
{
    if (user && !this->is_registered(user->get_nickname()))
        this->registered.push_back(user);
}

void Server::remove_registered(const std::string &nickname)
{
    if (this->is_registered(nickname))
    {
        for (std::vector<Channel *>::iterator c = this->channels.begin();
            c < this->channels.end(); c++)
        {
            (*c)->remove_invitation(nickname);
            (*c)->remove_operator(nickname);
            (*c)->remove_user(nickname);
        }
        this->registered.erase(this->get_registered(nickname));
    }
}

std::vector<User *>::iterator Server::get_registered(
    const std::string &nickname)
{
    return get_user_by_nickname(this->registered, nickname);
}

bool Server::is_registered(const std::string &nickname)
{
    return this->get_registered(nickname) != this->registered.end();
}

void Server::set_channel(Channel *c)
{
    if (c && !this->is_channel(c->get_topic()))
        this->channels.push_back(c);
}

void Server::remove_channel(const std::string &topic)
{
    if (this->is_channel(topic))
    {
        this->channels.erase(this->get_channel(topic));
    }
}

std::vector<Channel *>::iterator Server::get_channel(
    const std::string &topic)
{
    return get_channel_by_topic(this->channels, topic);
}

bool Server::is_channel(const std::string &topic)
{
    return this->get_channel(topic) != this->channels.end();
}

std::vector<User *>::iterator Server::get_user_by_fd(int fd)
{
    for (std::vector<User *>::iterator u = this->registered.begin();
        u != this->registered.end(); u++)
    {
        if ((*u)->get_fd() == fd)
            return u;
    }
    return this->registered.end();
}

void Server::print_server_status(const std::string &last_message) const
{
    static int number_of_requests = 0;
    if (last_message != "")
        number_of_requests ++;
    std::cout << " Clients connected: " << this->registered.size() << std::endl;
    std::cout << " Channels: " << this->channels.size() << std::endl;
    std::cout << " Requests received: " << number_of_requests << std::endl;
    for (unsigned int i = 0; i < 80 && i < last_message.size(); i++)
    {
        if (last_message[i] != '\t')
            std::cout << last_message[i];
    }
    std::cout << std::endl << "\033[4A";
}
