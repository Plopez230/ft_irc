/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 02:07:13 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/19 02:07:13 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void invite_command(Command *c, Server *s, User *u)
{
    if (c->size() < 3)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }

    std::string channel_name = c->argument(2);

    if (!s->is_channel(channel_name))
    {
        u->enqueue_message(err_nosuchchannel(s, channel_name, u));
        return;
    }

    Channel *channel = *s->find_channel(channel_name);

    if (!channel->is_operator(u->get_nickname()))
    {
        u->enqueue_message(err_chanoprivsneeded(s, channel, u));
        return;
    }

    if (!channel->is_user(u->get_nickname()))
    {
        u->enqueue_message(err_notonchannel(s, u, channel_name));
        return;
    }

    std::string nickname = c->argument(1);

    if (!is_valid_nickname(nickname) || !s->is_registered(nickname))
    {
        u->enqueue_message(err_nosuchnick(s, u, nickname));
        return;
    }

    User *user = *s->find_registered(nickname);

    if (channel->is_user(nickname))
    {
        u->enqueue_message(err_useronchannel(s, channel, u, user));
        return;
    }

    channel->add_invitation(user);
    u->enqueue_message(rpl_inviting(s, channel, u, user));
}
