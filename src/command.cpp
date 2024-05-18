/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 20:20:20 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/14 20:20:20 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static std::string remove_delimiters(std::string &s)
{
    unsigned int start = 0;
    unsigned int end = s.size();

    while (start < end && (s[start] == ' ' || s[start] == ':'))
    {
        start ++;
    }

    while (start < end && (s[end-1] == ' ' || s[end-1] == ':'))
    {
        end --;
    }

    return s.substr(start, end - start);
}

std::vector<std::string> split(
    const std::string &s, char del, bool include_delimiter)
{
    std::vector<std::string> result;
    unsigned int start = 0;
    unsigned int end = 0;

    while (start < s.size())
    {
        while (s[start] == del && start < s.size())
        {
            start ++;
        }
        
        end = start;

        if (start > 0 && include_delimiter)
        {
            start -= 1;
        }
        
        while (s[end] != del && end < s.size())
        {
            end ++;
        }
        
        if (start != end)
        {
            result.push_back(s.substr(start, end - start));
        }
        
        start = end + 1;
    }
    return result;
}

Command::Command(const std::string &message)
{
    this->message = message;
    std::vector<std::string> s, c;
    c = split(message, ':', true);

    for (std::vector<std::string>::iterator i = c.begin(); i != c.end(); i++)
    {
        if ((*i)[0] == ':')
        {
            this->arguments.push_back(remove_delimiters(*i));
        }
        else
        {
            s = split(*i, ' ', false);

            for (std::vector<std::string>::iterator j = s.begin();
                j != s.end(); j++)
            {
                this->arguments.push_back(remove_delimiters(*j));
            }
        }
    }
}

Command::~Command()
{}

std::vector<std::string> &Command::get_arguments()
{
    return this->arguments;
}

const std::string &Command::get_message() const
{
    return this->message;
}

void Command::run_command(Server *s, User *u)
{
    s->print_server_status(this->message);

    if (this->arguments.size() == 0)
    {
        return;
    }
    if (this->arguments[0] == "NICK")
    {
        nick_command(this, s, u);
    }
    else if (this->arguments[0] == "USER")
    {
        user_command(this, s, u);
    }
    else if (this->arguments[0] == "PASS")
    {
        pass_command(this, s, u);
    }
    else if (this->arguments[0] == "JOIN")
    {
        join_command(this, s, u);
    }
    else if (this->arguments[0] == "MODE")
    {
        mode_command(this, s, u);
    }
    else if (this->arguments[0] == "QUIT")
    {
        quit_command(this, s, u);
    }
    else if (this->arguments[0] == "PRIVMSG")
    {
        privmsg_command(this, s, u);
    }
    else if (this->arguments[0] == "PART")
    {
        part_command(this, s, u);
    }
    // if (this->arguments[0] == "DOWN")
    //     down_command(this, s, u);
    // else if (this->arguments[0] == "INVITE")
    //     invite_command(this, s, u);
    // else if (this->arguments[0] == "KICK")
    //     kick_command(this, s, u);
    // else if (this->arguments[0] == "NOTICE")
    //     notice_command(this, s, u);
    // else if (this->arguments[0] == "TOPIC")
    //     topic_command(this, s, u);
    // else if (this->arguments[0] == "UP")
    //     up_command(this, s, u);
    // else if (this->arguments[0] == "WHO")
    //     who_command(this, s, u);
    else
    {
        u->enqueue_message(err_unknowncommand(this, s, u));
    }
}
