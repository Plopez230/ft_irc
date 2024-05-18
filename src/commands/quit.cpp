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
    u->enqueue_message(command_reply(c, u));
    throw CloseConnection("Quit");
}
