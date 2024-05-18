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

void pass_command(Command *c, Server *s, User *u)
{
    if (c->size() < 2)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }

    if (u->get_is_registered())
    {
        u->enqueue_message(err_alreadyregistered(s, u));
        return;
    }

    u->set_pass(c->argument(1));
}
