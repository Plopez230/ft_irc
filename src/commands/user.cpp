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

void user_command(Command *c, Server *s, User *u)
{
    if (c->get_arguments().size() < 5)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }

    if (u->get_is_registered())
    {
        u->enqueue_message(err_alreadyregistered(s, u));
        return;
    }

    u->set_username(c->get_arguments()[1]);
    u->set_host(c->get_arguments()[2]);
    u->set_server(c->get_arguments()[3]);
    u->set_realname(c->get_arguments()[4]);
    register_user(s, u);
}
