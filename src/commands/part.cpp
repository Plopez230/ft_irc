/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:31:21 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/18 18:31:21 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void part_command(Command *c, Server *s, User *u)
{
    if (c->size() < 2)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }

    std::vector<std::string> channel_names = split(c->argument(1), ',',
        false);
    
    for (size_t i = 0; i < channel_names.size(); i++)
    {
        if (!s->is_channel(channel_names[i]))
        {
            u->enqueue_message(err_nosuchchannel(s, channel_names[i], u));
            continue;
        }

        Channel *channel = s->find_channel(channel_names[i]);

        if (!channel->is_user(u->get_nickname()))
        {
            u->enqueue_message(err_notonchannel(s, u, channel_names[i]));
            continue;
        }

        channel->enqueue_message(command_reply(c, u, channel_names[i]));
        channel->remove_user(u->get_nickname());
    }
}
