/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 12:10:00 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/16 12:10:00 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static std::string err_prefix(Server *s, User *u, const std::string &id)
{
    std::string msg = ":" + s->get_server_name() + " " + id;
    if (u->get_nickname() != "")
        msg += " " + u->get_nickname();
    else
        msg += " *";
    msg += " ";
    return msg;
}

std::string err_passwdmismatch(Server *s, User *u)
{
    return err_prefix(s, u, "464") + ":Password incorrect\n";
}

std::string err_unknowncommand(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "421") + c->get_arguments()[0]
        + " :Unknown command\n";
}

std::string err_nonicknamegiven(Server *s, User *u)
{
    return err_prefix(s, u, "431") + ":No nickname given\n";
}

std::string err_erroneusnickname(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "432") + c->get_arguments()[1]
        + " :Erroneus nickname\n";
}

std::string err_nicknameinuse(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "433") + c->get_arguments()[1]
        + " :Nickname is already in use\n";
}

std::string err_nickcollision(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "436") + c->get_arguments()[1]
        + " :Nickname collision KILL\n";
}

std::string err_needmoreparams(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "461") + c->get_arguments()[0]
        + " :Not enough parameters\n";
}

std::string err_alreadyregistered(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "462") + ":You may not reregister\n";
}

std::string rpl_welcome(Server *s, User *u)
{
    return err_prefix(s, u, "001") + "Welcome to the Internet Relay Network :"
        + u->get_nickname() + "!" + u->get_username() + "@" 
        + u->get_host() + "\n";
}

std::string rpl_yourhost(Server *s, User *u)
{
    return err_prefix(s, u, "002") + "Your host is " + u->get_server()
        + ", running version " + s->get_server_version() + "\n";
}

std::string rpl_created(Server *s, User *u)
{
    return err_prefix(s, u, "003") + "This server was created today :)\n";
}

std::string rpl_myinfo(Server *s, User *u)
{
    return err_prefix(s, u, "004") + u->get_server() + " " 
        + s->get_server_version() 
        + " Available user modes: , Available channel modes: iklot\n";
}
