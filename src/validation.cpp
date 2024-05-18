/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 15:06:45 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/18 15:06:45 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool is_valid_channel_name(const std::string &channel_name)
{
    return channel_name.length() > 2 && channel_name[0] == '#';
}

bool is_valid_nickname(const std::string &nickname)
{
    if (nickname.length() < 1 || nickname.length() > MAX_NICKNAME_LENGTH)
    {
        return false;
    }

    for (size_t p = 0; p < nickname.length(); p++)
    {
        char c = nickname[p];
        
        if (p == 0 && !((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || c == '`' || c == '|' || c == '^' || c == '_'
            || c == '{' || c == '}' || c == '[' || c == ']' || c == '\\'))
        {
            return false;
        }

        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9')
            || c == '`' || c == '|' || c == '^' || c == '_' || c == '-'
            || c == '{' || c == '}' || c == '[' || c == ']' || c == '\\'))
        {
            return false;
        }
    }
    return true;
}

std::string to_lower(const std::string &nickname)
{
    std::string lower(nickname);

    for (size_t p = 0; p < nickname.length(); p++)
    {
        char c = nickname[p];

        if (c >= 'A' && c <= 'Z')
        {
            lower[p] = nickname[p] - 'A' + 'a';
        }
    }
    return lower;
}

void register_user(Server *s, User *u)
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

        u->set_is_registered(true);
        u->enqueue_message(":" + user_jid(u) + " NICK " + u->get_nickname());
        u->enqueue_message(rpl_welcome(s, u));
        u->enqueue_message(rpl_yourhost(s, u));
        u->enqueue_message(rpl_created(s, u));
        u->enqueue_message(rpl_myinfo(s, u));
    }
}
