/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 11:22:41 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/18 11:22:41 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <cstdio>

static void mode_command_query(Server *s, Channel *channel, User *u)
{
    std::string mode_string;

    if (channel->has_mode(MODE_I))
    {
        mode_string += "i";
    }

    if (channel->has_mode(MODE_L))
    {
        mode_string += "l";
    }

    if (channel->has_mode(MODE_K))
    {
        mode_string += "k";
    }

    if (channel->has_mode(MODE_T))
    {
        mode_string += "t";
    }

    if (mode_string != "")
    {
        mode_string = "+" + mode_string;
    }

    u->enqueue_message(rpl_channelmodeis(s, channel, u, mode_string));
    return;
}

static void mode_command_modify_i(Command *c, Channel *channel, User *u,
    bool flag_sign)
{
    if (flag_sign)
    {
        channel->set_mode(MODE_I);
        channel->enqueue_message(command_reply(c, u, channel->get_name() 
            + " +i"));
    }

    else
    {
        channel->unset_mode(MODE_I);
        channel->enqueue_message(command_reply(c, u, channel->get_name() 
            + " -i"));
    }
}

static void mode_command_modify_t(Command *c, Channel *channel, User *u,
    bool flag_sign)
{
    if (flag_sign)
    {
        channel->set_mode(MODE_T);
        channel->enqueue_message(command_reply(c, u, channel->get_name() 
            + " +t"));
    }

    else
    {
        channel->unset_mode(MODE_T);
        channel->enqueue_message(command_reply(c, u, channel->get_name()
            + " -t"));
    }
}

static void mode_command_modify_k(Command *c, Server *s, Channel *channel,
    User *u, bool flag_sign, size_t argument_pos)
{
    if (flag_sign)
    {
        if (c->size() <= argument_pos)
        {
            u->enqueue_message(err_needmoreparams(c, s, u));
            return;
        }

        if (channel->has_mode(MODE_K))
        {
            u->enqueue_message(err_keyset(s, channel, u));
            return;
        }

        std::string channel_key = c->argument(argument_pos);

        channel->set_mode(MODE_K);
        channel->set_pass(channel_key);
        channel->enqueue_message(command_reply(c, u, channel->get_name()
            + " +k " + channel_key));
    }

    else
    {
        channel->unset_mode(MODE_K);
        channel->set_pass("");
        channel->enqueue_message(command_reply(c, u, channel->get_name()
            + " -k"));
    }
}

static void mode_command_modify_l(Command *c, Server *s, Channel *channel,
    User *u, bool flag_sign, size_t argument_pos)
{
    if (flag_sign)
    {
        if (c->size() <= argument_pos)
        {
            u->enqueue_message(err_needmoreparams(c, s, u));
            return;
        }

        std::string argument = c->argument(argument_pos);
        size_t channel_limit;

        if (!sscanf(argument.c_str(), " %lu", &channel_limit))
        {
            u->enqueue_message(err_unknownmode(s, u, 'l'));
            return;
        }

        channel->set_mode(MODE_L);
        channel->set_max_users(channel_limit);
        channel->enqueue_message(command_reply(c, u, channel->get_name()
            + " +l " + argument));
    }

    else
    {
        channel->unset_mode(MODE_L);
        channel->enqueue_message(command_reply(c, u, channel->get_name()
            + " -l"));
    }
}

static void mode_command_modify_o(Command *c, Server *s, Channel *channel,
    User *u, bool flag_sign, size_t argument_pos)
{
    if (c->size() <= argument_pos)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }
    
    std::string nickname = c->argument(argument_pos);

    if (!s->is_registered(nickname))
    {
        u->enqueue_message(err_nosuchnick(s, u, nickname));
        return;
    }

    User *to_op = s->find_registered(nickname);

    if (flag_sign && !channel->is_operator(nickname))
    {
        channel->add_operator(to_op);
        channel->enqueue_message(command_reply(c, u, channel->get_name() 
            + " +o " + nickname));
    }

    else if (channel->is_operator(nickname))
    {
        channel->remove_operator(nickname);
        channel->enqueue_message(command_reply(c, u, channel->get_name()
            + " -o " + nickname));
    }
}

static void mode_command_modify(Command *c, Server *s, Channel *channel,
    User *u)
{
    if (!channel->is_operator(u->get_nickname()))
    {
        u->enqueue_message(err_chanoprivsneeded(s, channel, u));
        return;
    }

    std::string mode_string = c->argument(2);
    bool flag_sign = true;
    size_t argument_pos = 3;

    for (size_t pos = 0; pos < mode_string.length(); pos++)
    {
        if (mode_string[pos] == '+')
        {
            flag_sign = true;
        }

        else if (mode_string[pos] == '-')
        {
            flag_sign = false;
        }

        else if (mode_string[pos] == 'i')
        {
            mode_command_modify_i(c, channel, u, flag_sign);
        }

        else if (mode_string[pos] == 't')
        {
            mode_command_modify_t(c, channel, u, flag_sign);
        }

        else if (mode_string[pos] == 'k')
        {
            mode_command_modify_k(c, s, channel, u, flag_sign, argument_pos);
            argument_pos++;
        }

        else if (mode_string[pos] == 'l')
        {
            mode_command_modify_l(c, s, channel, u, flag_sign, argument_pos);
            argument_pos++;
        }

        else if (mode_string[pos] == 'o')
        {
            mode_command_modify_o(c, s, channel, u, flag_sign, argument_pos);
            argument_pos++;
        }

        else
        {
            u->enqueue_message(err_unknownmode(s, u, mode_string[pos]));
        }
    }
}

void mode_command(Command *c, Server *s, User *u)
{
    if (c->size() < 2)
    {
        u->enqueue_message(err_needmoreparams(c, s, u));
        return;
    }

    std::string channel_name = c->argument(1);

    if (!is_valid_channel_name(channel_name))
    {
        u->enqueue_message(err_nosuchnick(s, u, channel_name));
        return;
    }

    if (!s->is_channel(channel_name))
    {
        u->enqueue_message(err_nosuchnick(s, u, channel_name));
        return;
    }

    Channel *channel = s->find_channel(channel_name);

    if (!channel->is_user(u->get_nickname()))
    {
        u->enqueue_message(err_notonchannel(s, u, channel_name));
        return;
    }

    else if (c->size() == 2)
    {
        mode_command_query(s, channel, u);
        return;
    }

    else
    {
        mode_command_modify(c, s, channel, u);
        return;
    }
}
