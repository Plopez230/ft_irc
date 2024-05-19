/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 16:47:48 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/18 16:47:48 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void quit_command(Command *c, Server *s, User *u)
{
    (void) s;
    s->enqueue_message_in_user_channels(command_reply(c, u), u);
    throw CloseConnection("Quit");
}
