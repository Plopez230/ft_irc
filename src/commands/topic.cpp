/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 02:36:43 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/27 15:25:21 by jariza-o         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void topic_command(Command *c, Server *s, User *u)
{
    if (!u->get_is_registered())
    {
        u->enqueue_message(err_notregistered(s, u));
        return;
    }
    
    if (c->size() < 2)
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

    if (!channel->is_user(u->get_nickname()))
    {
        u->enqueue_message(err_notonchannel(s, u, channel_name));
        return;
    }

    if (c->size() > 2)
    {
        if (channel->has_mode(MODE_T)
            && !channel->is_operator(u->get_nickname()))
        {
            u->enqueue_message(err_chanoprivsneeded(s, channel, u));
            u->enqueue_message(rpl_topic(s, channel, u));
            return;
        }

        std::string topic = c->argument(2);
        channel->set_topic(topic.substr(0, 307));
        channel->enqueue_message(command_reply(c, u));
    }

    if (channel->get_topic() == "")
    {
        channel->enqueue_message(rpl_notopic(s, channel, u));
    }

    else
    {
        channel->enqueue_message(rpl_topic(s, channel, u));
    }
}