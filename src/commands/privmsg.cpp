/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 17:47:53 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/18 17:47:53 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void privmsg_command(Command *c, Server *s, User *u)
{
    if (c->size() < 3)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }

    std::vector<std::string> receivers = split(c->argument(1), ',',
        false);
    std::string message = c->argument(2);

    if (message.length() == 0)
    {
        u->enqueue_message(err_notexttosend(s, u));
    }

    if (receivers.size() == 0)
    {
        u->enqueue_message(err_norecipient(c, s, u));
        return;
    }

    for (size_t i = 0; i < receivers.size(); i++)
    {
        if (s->is_channel(receivers[i]))
        {
            Channel *channel = s->find_channel(receivers[i]);
            channel->enqueue_message(
                command_reply(c, u, receivers[i] + " :" + message), u);
        }

        else if (s->is_registered(receivers[i]))
        {
            User *user = s->find_registered(receivers[i]);
            user->enqueue_message(
                command_reply(c, u, receivers[i] + " :" + message));
        }

        else
        {
            u->enqueue_message(err_nosuchnick(s, u, receivers[i]));
        }
    }
}
