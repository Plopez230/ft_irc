/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_manager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 00:19:08 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/15 00:19:08 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <stdexcept>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

SocketManager::SocketManager(int port, Server &server):
    port(port),
    server(server)
{
    this->manager_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->manager_fd < 0)
        throw std::runtime_error("Unable to create socket");
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port);
    int enabled = 1;
    if (setsockopt(
        this->manager_fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(int)) < 0)
        throw std::runtime_error("Failed to reuse address");
    if (fcntl(this->manager_fd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Unable to set nonblocking socket");
    if (bind(this->manager_fd, (struct sockaddr*)&this->address,
        sizeof(this->address)) < 0)
        throw std::runtime_error("Unable to bind socket");
    if (listen(this->manager_fd, 42) < 0)
        throw std::runtime_error("Unable to listen");
    pollfd socketPoll = {this->manager_fd, POLLIN, 0};
    this->pollfds.push_back(socketPoll);
}

SocketManager::~SocketManager()
{
    std::vector<pollfd>::iterator i = this->pollfds.begin();
    for (; i < this->pollfds.end(); i++)
        close((*i).fd);
    close(this->manager_fd);
}

void SocketManager::loop()
{
    std::vector<pollfd> to_close;
    if (poll(&(this->pollfds)[0], this->pollfds.size(), -1) == -1)
        throw std::runtime_error("Poll function failed");
    for (size_t i = 0; i < this->pollfds.size(); i++)
    {
        if (this->pollfds[i].revents == 0)
            continue;
        if (this->pollfds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
            to_close.push_back(this->pollfds[i]);
        else if (this->pollfds[i].fd == this->manager_fd)
            new_connection(this->pollfds[i]);
        else
        {
            if (this->pollfds[i].revents & POLLIN)
                this->receive_message(this->pollfds[i], to_close);
            if (this->pollfds[i].revents & POLLOUT)
                this->send_messages(this->pollfds[i], to_close);
        }
    }
    std::vector<pollfd>::iterator i = to_close.begin();
    for (; i < to_close.end(); i++)
        this->end_connection(*i);
}

void SocketManager::new_connection(pollfd pfd)
{
    socklen_t size = sizeof(this->address);
    int client_socket = accept(pfd.fd, 
        (struct sockaddr*) &this->address, &size);
    if (client_socket < 0)
        throw std::runtime_error("Unable to accept connection");
    if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("Unable to set nonblocking socket");
    pollfd new_pollfd = {client_socket, POLLIN | POLLOUT, 0};
    this->pollfds.push_back(new_pollfd);
    User *new_user = new User();
    new_user->set_fd(client_socket);
    this->server.add_registered(new_user);
    this->server.print_server_status("");
}

void SocketManager::end_connection(pollfd pfd)
{
    if (pfd.fd != -1) {
        std::vector<pollfd>::iterator i = pollfds.begin();
        while (i != pollfds.end() && (*i).fd != pfd.fd)
            ++i;
        if (i != pollfds.end()) {
            close((*i).fd);
            pollfds.erase(i);
            this->server.remove_registered(pfd.fd);
            this->server.print_server_status("");
        }
    }
}

void SocketManager::receive_message(pollfd pfd, std::vector<pollfd> &to_close)
{
    char message_buffer[BUFFER_SIZE];
    std::memset(message_buffer, '\0', BUFFER_SIZE);
    int readBytes = recv(pfd.fd, message_buffer, BUFFER_SIZE - 1, 0);
    if (readBytes <= 0)
        to_close.push_back(pfd);
    else
    {
        User *user = *this->server.find_user_by_fd(pfd.fd);
        std::string u_msg_buffer = user->get_input_buffer() + message_buffer;
        size_t end_of_command = u_msg_buffer.find("\n");
        if (end_of_command != std::string::npos)
        {
            std::string command_string = u_msg_buffer.substr(0, end_of_command);
            u_msg_buffer = u_msg_buffer.substr(
                end_of_command + 1, u_msg_buffer.size() - end_of_command - 1);
            Command command(command_string);
            try
            {
                command.run_command(&this->server, user);
            }
            catch (CloseConnection &c)
            {
                to_close.push_back(pfd);
            }
        }
        user->set_input_buffer(u_msg_buffer);
    }
}

void SocketManager::send_messages(pollfd pfd, std::vector<pollfd> &to_close)
{
    User *user = *this->server.find_user_by_fd(pfd.fd);
    if (!user)
        return;
    while (user->has_queued_messages())
    {
        std::string message = user->dequeue_message();
        if (send(pfd.fd, message.c_str(), message.size(), 0) < 0) {
            to_close.push_back(pfd);
            break;
        }
    }
}

CloseConnection::CloseConnection(const std::string &what):
    std::runtime_error(what)
{}

CloseConnection::~CloseConnection()
{}

StopServer::StopServer(const std::string &what):
    std::runtime_error(what)
{}

StopServer::~StopServer()
{}
