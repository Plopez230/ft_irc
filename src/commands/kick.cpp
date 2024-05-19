/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 19:02:39 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/18 19:02:39 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void kick_command(Command *c, Server *s, User *u)
{
    if (c->size() < 3)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }

    std::string channel_name = c->argument(1);

    if (!s->is_channel(channel_name))
    {
        u->enqueue_message(err_nosuchchannel(s, channel_name, u));
        return;
    }

    Channel *channel = s->find_channel(channel_name);

    if (!channel->is_operator(u->get_nickname()))
    {
        u->enqueue_message(err_chanoprivsneeded(s, channel, u));
        return;
    }

    std::string username = c->argument(2);

    if (!channel->is_user(username))
    {
        u->enqueue_message(err_notonchannel(s, u, channel_name));
        return;
    }

    channel->enqueue_message(command_reply(c, u));
    channel->remove_user(username);
    channel->remove_invitation(username);
    channel->remove_operator(username);
}
