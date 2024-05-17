/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auth.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 11:36:50 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/16 11:36:50 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static bool is_valid_nickname(std::string &nickname)
{
    for (size_t p = 0; p < nickname.length(); p++)
    {
        char c = nickname[p];
        if (p == 0 && !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || c == '`' || c == '|' || c == '^' || c == '_'
            || c == '{' || c == '}' || c == '[' || c == ']' || c == '\\'))
            return false;
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9')
            || c == '`' || c == '|' || c == '^' || c == '_' || c == '-'
            || c == '{' || c == '}' || c == '[' || c == ']' || c == '\\'))
            return false;
    }
    return true;
}

std::string nickname_to_lower(const std::string &nickname)
{
    std::string lower(nickname);
    for (size_t p = 0; p < nickname.length(); p++)
    {
        char c = nickname[p];
        if (c >= 'A' && c <= 'Z')
            lower[p] = nickname[p] - 'A' + 'a';
    }
    return lower;
}

static void authenticate(Server *s, User *u)
{
    if (u->get_nickname() != "" && u->get_username() != ""
        && u->get_server() != "" && u->get_host() != ""
        && u->get_realname() != "")
    {
        if (u->get_pass() != s->get_pass())
        {
            u->enqueue_message(err_passwdmismatch(s, u));
            throw CloseConnection("Bad password");
        }
        u->set_is_authenticated(true);
        u->enqueue_message(":" + user_jid(u) + " NICK " + u->get_nickname());
        u->enqueue_message(rpl_welcome(s, u));
        u->enqueue_message(rpl_yourhost(s, u));
        u->enqueue_message(rpl_created(s, u));
        u->enqueue_message(rpl_myinfo(s, u));
    }
}

void nick_command(Command *c, Server *s, User *u)
{
    if (c->get_arguments().size() < 2 || c->get_arguments()[1].length() == 0)
    {
        u->enqueue_message(err_nonicknamegiven(s, u));
        return;
    }
    if (!is_valid_nickname(c->get_arguments()[1]))
    {
        u->enqueue_message(err_erroneusnickname(c, s, u));
        return;
    }
    if (c->get_arguments()[1] == u->get_nickname()
        || s->is_registered(c->get_arguments()[1]))
    {
        u->enqueue_message(err_nicknameinuse(c, s, u));
        return;
    }
    u->set_nickname(c->get_arguments()[1]);
    authenticate(s, u);
}

void pass_command(Command *c, Server *s, User *u)
{
    if (c->get_arguments().size() < 2)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }
    if (u->get_is_authenticated())
    {
        u->enqueue_message(err_alreadyregistered(s, u));
        return;
    }
    u->set_pass(c->get_arguments()[1]);
}

void user_command(Command *c, Server *s, User *u)
{
    if (c->get_arguments().size() < 5)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }
    if (u->get_is_authenticated())
    {
        u->enqueue_message(err_alreadyregistered(s, u));
        return;
    }
    u->set_username(c->get_arguments()[1]);
    u->set_host(c->get_arguments()[2]);
    u->set_server(c->get_arguments()[3]);
    u->set_realname(c->get_arguments()[4]);
    authenticate(s, u);
}
