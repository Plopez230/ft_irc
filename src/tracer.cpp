/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:25:15 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/18 15:25:15 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <cstdlib>

Tracer::Tracer()
{
    this->enabled = false;

    if (std::getenv("IRC_LOG_ENABLED"))
    {
        this->file = new std::ofstream("trace.log");
        this->enabled = true;
    }
}

Tracer::~Tracer()
{
    if (this->enabled)
    {
        this->file->close();
        delete this->file;
    }
}

void Tracer::trace_input(int fd, std::string msg)
{
    if (msg != "" && this->enabled)
    {
        *this->file << ">" << fd << ":" << msg << std::endl;
    }
}

void Tracer::trace_output(int fd, std::string msg)
{
    if (msg != "" && this->enabled)
    {
        *this->file << "<" << fd << ":" << msg << std::endl;
    }
}

void Tracer::end_connection(int fd)
{
    if (this->enabled)
    {
        *this->file << "x" << fd << ":" << std::endl;
    }
}
