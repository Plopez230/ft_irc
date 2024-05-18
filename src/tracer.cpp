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
#include <fstream>

Tracer::Tracer()
{
    this->file = new std::ofstream("trace.log");
}

Tracer::~Tracer()
{
    this->file->close();
    delete this->file;
}

void Tracer::trace_input(int fd, std::string msg)
{
    if (msg != "")
    {
        *this->file << ">" << fd << ":" << msg << std::endl;
    }
}

void Tracer::trace_output(int fd, std::string msg)
{
    if (msg != "")
    {
        *this->file << "<" << fd << ":" << msg << std::endl;
    }
}

void Tracer::end_connection(int fd)
{
    *this->file << "x" << fd << ":" << std::endl;
}
