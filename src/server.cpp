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
    
    std::cout << "\n\n\n\n";
    this->print_server_status("");
}

Server::~Server()
{
    for (std::vector<User *>::iterator u = this->registered.begin();
        u != this->registered.end(); u++)
    {
        delete *u;
    }

    for (std::vector<Channel *>::iterator c = this->channels.begin();
        c != this->channels.end(); c++)
    {
        delete *c;
    }
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

void Server::add_registered(User *user)
{
    if (user)
    {
        this->registered.push_back(user);
    }
}

void Server::remove_registered(const std::string &nickname)
{
    if (this->is_registered(nickname))
    {
        User *u = this->find_registered(nickname);

        for (std::vector<Channel *>::iterator c = this->channels.begin();
            c < this->channels.end(); c++)
        {
            (*c)->remove_invitation(nickname);
            (*c)->remove_operator(nickname);
            (*c)->remove_user(nickname);
        }

        this->registered.erase(
            find_user_by_nickname(this->registered, nickname));
        delete u;
    }
}

void Server::remove_registered(int fd)
{
    User *user = this->find_registered(fd);

    for (size_t i = 0; i < this->channels.size(); i++)
    {
        Channel *c = this->channels[i];

        c->remove_invitation(user->get_nickname());
        c->remove_user(user->get_nickname());
        c->remove_operator(user->get_nickname());
    }

    this->registered.erase(find_user_by_fd(this->registered, fd));
    delete user;
}

User *Server::find_registered(const std::string &nickname)
{
    return *find_user_by_nickname(this->registered, nickname);
}

User *Server::find_registered(int fd)
{
    return *find_user_by_fd(this->registered, fd);
}

bool Server::is_registered(const std::string &nickname)
{
    return find_user_by_nickname(this->registered, nickname)
        != this->registered.end();
}

void Server::add_channel(Channel *c)
{
    if (c && !this->is_channel(c->get_name()))
    {
        this->channels.push_back(c);
    }
}

void Server::remove_channel(const std::string &channel_name)
{
    if (this->is_channel(channel_name))
    {
        Channel *channel = this->find_channel(channel_name);

        this->channels.erase(find_channel_by_name(
            this->channels, channel_name));
        delete channel;
    }
}

Channel *Server::find_channel(const std::string &channel_name)
{
    return *find_channel_by_name(this->channels, channel_name);
}

bool Server::is_channel(const std::string &topic)
{
    return find_channel_by_name(this->channels, topic) != this->channels.end();
}

void Server::print_server_status(const std::string &last_message) const
{
    static int number_of_requests = 0;
    static std::string printed_message = "";

    if (last_message != "")
    {
        number_of_requests ++;
        printed_message = last_message;
    }

    std::cout   << "\r\033[4A"
                << "\033[KClients connected:   \033[1;32m" 
                << this->registered.size() << "\033[0m" << std::endl
                << "\033[KChannels:            \033[1;32m" 
                << this->channels.size() << "\033[0m" << std::endl
                << "\033[KRequests received:   \033[1;32m" 
                << number_of_requests << "\033[0m" << std::endl
                << "\033[K\033[1;36m";

    for (unsigned int i = 0; i < 80 && i < printed_message.size(); i++)
    {
        if (printed_message[i] != '\t')
        {
            std::cout << printed_message[i];
        }
    }

    std::cout << std::endl << "\033[0m";
}

void Server::enqueue_message_in_user_channels(
    const std::string &msg, User *user)
{
    for (size_t i = 0; i < this->channels.size(); i++)
    {
        Channel *channel = this->channels[i];

        if (channel->is_user(user->get_nickname()))
        {
            channel->enqueue_message(msg);
        }
    }
}
