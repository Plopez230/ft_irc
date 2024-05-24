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

void nick_command(Command *c, Server *s, User *u)
{
    if (c->size() < 2 || c->argument(1).length() == 0)
    {
        u->enqueue_message(err_nonicknamegiven(s, u));
        return;
    }

    if (!is_valid_nickname(c->argument(1)))
    {
        u->enqueue_message(err_erroneusnickname(c, s, u));
        return;
    }

    if (c->argument(1) == u->get_nickname()
        || s->is_registered(c->argument(1)))
    {
        u->enqueue_message(err_nicknameinuse(c, s, u));
        return;
    }

    if (u->get_is_registered())
    {
        s->enqueue_message_in_user_channels(command_reply(c, u), u);
        u->enqueue_message(command_reply(c, u));
    }

    u->set_nickname(c->argument(1));

    if (!u->get_is_registered())
    {
        register_user(s, u);
    }
}
