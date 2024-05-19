/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 15:27:58 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/16 15:27:58 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

static void join_channel(Server *s, User *u, const std::string &channel_name,
    const std::string &channel_key)
{
    Channel *channel;

    if (!is_valid_channel_name(channel_name))
    {
        u->enqueue_message(err_nosuchchannel(s, channel_name, u));
        return;
    }

    if (!s->is_channel(channel_name))
    {
        channel = new Channel();
        channel->set_name(channel_name);
        channel->set_topic("");
        channel->set_pass(channel_key);
        channel->add_user(u);
        channel->add_operator(u);
        s->add_channel(channel);

        u->enqueue_message(":" + user_jid(u) + " JOIN :" + channel_name);
        u->enqueue_message(rpl_notopic(s, channel, u));
        u->enqueue_message(rpl_namreply(s, channel, u));
        u->enqueue_message(rpl_endofnames(s, channel, u));

        return;
    }

    channel = s->find_channel(channel_name);

    if (channel->has_mode(MODE_I) && !channel->is_invitation(u->get_nickname()))
    {
        u->enqueue_message(err_inviteonlychan(s, channel, u));
        return;
    }

    if (channel->has_mode(MODE_K) && channel->get_pass() != channel_key)
    {
        u->enqueue_message(err_badchannelkey(s, channel, u));
        return;
    }

    if (channel->has_mode(MODE_L) && channel->is_full())
    {
        u->enqueue_message(err_channelisfull(s, channel, u));
        return;
    }

    channel->enqueue_message(":" + user_jid(u) + " JOIN :" + channel_name);
    channel->add_user(u);

    u->enqueue_message(":" + user_jid(u) + " JOIN :" + channel_name);
    u->enqueue_message(rpl_topic(s, channel, u));
    u->enqueue_message(rpl_namreply(s, channel, u));
    u->enqueue_message(rpl_endofnames(s, channel, u));
}

void join_command(Command *c, Server *s, User *u)
{
    if (c->size() < 2)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }

    std::vector<std::string> channel_names = split(
        c->argument(1), ',', false);
    
    std::vector<std::string> channel_keys;

    if (c->size() >= 3)
    {
        channel_keys = split(c->argument(2), ',', false);
    }

    for (size_t i = 0; i < channel_names.size(); i++)
    {
        std::string channel_name = channel_names[i];
        std::string channel_key = "";

        if (i < channel_keys.size())
        {
            channel_key = channel_keys[i];
        }

        join_channel(s, u, channel_name, channel_key);
    }
}
