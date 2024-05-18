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
    register_user(s, u);
}