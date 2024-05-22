/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_manager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 18:29:31 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/17 18:29:31 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

SocketManager::SocketManager(char *port, Server &server):
	port(port),
	server(server)
{
	this->hints.ai_family = AF_INET;
	this->hints.ai_socktype = SOCK_STREAM;
	this->hints.ai_protocol = IPPROTO_TCP;
	this->hints.ai_flags = AI_PASSIVE;

	int gai_return = getaddrinfo("0.0.0.0", port, &this->hints, &this->res);
	if (gai_return < 0)
	{
		throw std::runtime_error(gai_strerror(gai_return));
	}

	this->manager_fd = socket(this->res->ai_family, this->res->ai_socktype,
		this->res->ai_protocol);

	if (this->manager_fd < 0)
	{
		throw std::runtime_error("socket failed");
	}

	int optvalue = 1;

	if (setsockopt(this->manager_fd, SOL_SOCKET, SO_REUSEADDR, &optvalue,
		sizeof(optvalue)) < 0)
	{
		throw std::runtime_error("reuse address failed");
	}
	
	if (fcntl(this->manager_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		throw std::runtime_error("set nonblocking socket failed");
	}

	if (bind(this->manager_fd, this->res->ai_addr, this->res->ai_addrlen) < 0)
	{
		throw std::runtime_error("bind socket failed");
	}

	if (listen(this->manager_fd, MAX_CONNECTIONS) < 0)
	{
		throw std::runtime_error("Unable to listen");
	}
	
	freeaddrinfo(this->res);

	pollfd socketPoll = {this->manager_fd, POLLIN, 0};
	this->pollfds.push_back(socketPoll);
}

SocketManager::~SocketManager()
{
	for (std::vector<pollfd>::iterator i = this->pollfds.begin();
		i < this->pollfds.end(); i++)
	{
		close((*i).fd);
	}
	close(this->manager_fd);
}

void SocketManager::loop()
{
	std::vector<pollfd> to_close;

	if (poll(&(this->pollfds)[0], this->pollfds.size(), -1) == -1)
	{
		throw std::runtime_error("poll failed");
	}

	for (std::vector<pollfd>::iterator i = this->pollfds.begin();
		i < this->pollfds.end(); i++)
	{
		if (i->revents == 0)
		{
			continue;
		}

		if (i->revents & (POLLHUP | POLLERR | POLLNVAL))
		{
			to_close.push_back(*i);
		}

		else if (i->fd == this->manager_fd)
		{
			new_connection(*i);
		}

		else
		{
			if (i->revents & POLLIN)
			{
				this->receive_message(*i, to_close);
			}

			if (i->revents & POLLOUT)
			{
				this->send_messages(*i, to_close);
			}
		}
	}

	for (std::vector<pollfd>::iterator i = to_close.begin();
		i < to_close.end(); i++)
	{
		this->end_connection(*i);
	}
}

void SocketManager::new_connection(pollfd pfd)
{
	sockaddr_in client_sockaddr;
	socklen_t addr_size = sizeof(sockaddr_in);

	int client_socket = accept(pfd.fd, (sockaddr *)&client_sockaddr,
		&addr_size);

	if (client_socket < 0)
	{
		throw std::runtime_error("Unable to accept connection");
	}

	if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		throw std::runtime_error("Unable to set nonblocking socket");
	}

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
		{
			++i;
		}

		if (i != pollfds.end())
		{
			this->tracer.end_connection(pfd.fd);
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
	{
		to_close.push_back(pfd);
	}
	else
	{
		User *user = this->server.find_registered(pfd.fd);

		std::string u_msg_buffer = user->get_input_buffer() + message_buffer;
		size_t end_of_command =  u_msg_buffer.find_first_of("\r\n");

		while (end_of_command != std::string::npos)
		{
			std::string command_string = u_msg_buffer.substr(0,
				end_of_command);

			u_msg_buffer = u_msg_buffer.substr(
				end_of_command + 1,
				u_msg_buffer.size() - end_of_command - 1);

			this->tracer.trace_input(pfd.fd, command_string);

			Command command(command_string);
			try
			{
				command.run_command(&this->server, user);
			}
			catch (CloseConnection &c)
			{
				to_close.push_back(pfd);
			}

			end_of_command = u_msg_buffer.find_first_of("\r\n");
		}
		user->set_input_buffer(u_msg_buffer);
	}
}

void SocketManager::send_messages(pollfd pfd, std::vector<pollfd> &to_close)
{
	User *user = this->server.find_registered(pfd.fd);

	if (!user)
	{
		return;
	}

	while (user->has_queued_messages())
	{
		std::string message = user->dequeue_message();
		
		this->tracer.trace_output(pfd.fd, message);
		
		message += "\n";

		if (send(pfd.fd, message.c_str(), message.size(), 0) < 0)
		{
			to_close.push_back(pfd);
			break;
		}
	}
}

CloseConnection::CloseConnection(const std::string &what):
	std::runtime_error(what)
{}

StopServer::StopServer(const std::string &what):
	std::runtime_error(what)
{}
