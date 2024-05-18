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

std::string user_jid(User *u)
{
    return u->get_nickname() + "!" + u->get_username() + "@" + u->get_host();
}

static std::string err_prefix(Server *s, User *u, const std::string &id)
{
    std::string msg = ":" + s->get_server_name() + " " + id;

    if (u->get_nickname() != "")
    {
        msg += " " + u->get_nickname();
    }

    else
    {
        msg += " *";
    }

    msg += " ";
    
    return msg;
}

std::string err_passwdmismatch(Server *s, User *u)
{
    return err_prefix(s, u, "464") + ":Password incorrect";
}

std::string err_unknowncommand(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "421") + c->get_arguments()[0]
        + " :Unknown command";
}

std::string err_nonicknamegiven(Server *s, User *u)
{
    return err_prefix(s, u, "431") + ":No nickname given";
}

std::string err_erroneusnickname(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "432") + c->get_arguments()[1]
        + " :Erroneus nickname";
}

std::string err_nicknameinuse(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "433") + c->get_arguments()[1]
        + " :Nickname is already in use";
}

std::string err_nickcollision(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "436") + c->get_arguments()[1]
        + " :Nickname collision KILL";
}

std::string err_needmoreparams(Command *c, Server *s, User *u)
{
    return err_prefix(s, u, "461") + c->get_arguments()[0]
        + " :Not enough parameters";
}

std::string err_alreadyregistered(Server *s, User *u)
{
    return err_prefix(s, u, "462") + ":You may not reregister";
}

std::string rpl_welcome(Server *s, User *u)
{
    return err_prefix(s, u, "001") + ":Welcome to the Internet Relay Network :"
        + u->get_nickname() + "!" + u->get_username() + "@" 
        + u->get_host();
}

std::string rpl_yourhost(Server *s, User *u)
{
    return err_prefix(s, u, "002") + ":Your host is " + u->get_server()
        + ", running version " + s->get_server_version();
}

std::string rpl_created(Server *s, User *u)
{
    return err_prefix(s, u, "003") + ":This server was created today :)";
}

std::string rpl_myinfo(Server *s, User *u)
{
    return err_prefix(s, u, "004") + u->get_server() + " " 
        + s->get_server_version() 
        + " Available user modes: , Available channel modes: iklot";
}

std::string rpl_topic(Server *s, Channel *c, User *u)
{
    return err_prefix(s, u, "332") + c->get_name() + " :" + c->get_topic();
}

std::string err_inviteonlychan(Server *s, Channel *c, User *u)
{
    return err_prefix(s, u, "473") + c->get_name()
        + " :Cannot join channel (+i)";
}

std::string err_badchannelkey(Server *s, Channel *c, User *u)
{
    return err_prefix(s, u, "475") + c->get_name()
        + " :Cannot join channel (+k)";
}

std::string err_channelisfull(Server *s, Channel *c, User *u)
{
    return err_prefix(s, u, "471") + c->get_name()
        + " :Cannot join channel (+l)";
}

std::string err_nosuchchannel(Server *s, const std::string &c, User *u)
{
    return err_prefix(s, u, "403") + c + " :No such channel";
}

std::string rpl_namreply(Server *s, Channel *c, User *u)
{
    return err_prefix(s, u, "353") + "= " + c->get_name()
        + " :" + c->get_nicknames();
}

std::string rpl_endofnames(Server *s, Channel *c, User *u)
{
    return err_prefix(s, u, "366") + c->get_name()
        + " :End of NAMES list";
}

std::string err_notonchannel(Server *s, User *u, const std::string &ch)
{
    return err_prefix(s, u, "442") + ch + " :You're not on that channel";
}

std::string err_unknownmode(Server *s, User *u, char mode)
{
    return err_prefix(s, u, "472") + mode + " :is unknown mode char to me";
}

std::string rpl_channelmodeis(Server *s, Channel *c, User *u,
    const std::string &mode)
{
    return err_prefix(s, u, "324") + c->get_name() + " " + mode;
}

std::string err_keyset(Server *s, Channel *c, User *u)
{
    return err_prefix(s, u, "467") + c->get_name()
        + " :Channel key already set";
}

std::string err_nosuchnick(Server *s, User *u, const std::string &nickname)
{
    return err_prefix(s, u, "401") + nickname + " :No such nick/channel";
}

std::string err_chanoprivsneeded(Server *s, Channel *c, User *u)
{
    return err_prefix(s, u, "482") + c->get_name()
        + " :You're not channel operator";
}

std::string command_reply(Command *c, User *u)
{
    return ":" + user_jid(u) + " " + c->get_message();
}

std::string command_reply(Command *c, User *u, const std::string &args)
{
    return ":" + user_jid(u) + " " + c->get_arguments()[0] + " " + args;
}
