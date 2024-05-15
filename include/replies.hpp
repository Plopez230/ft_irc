/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plopez-b <plopez-b@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 20:41:23 by plopez-b          #+#    #+#             */
/*   Updated: 2024/05/14 20:41:23 by plopez-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLIES_HPP
#define REPLIES_HPP

#define ERR_UNKNOWNCOMMAND_421(command_name, server, user) \
    ":" + (server)->get_server_name() + " 421 " + (user)->get_nickname() \
    + (command_name) + " :Unknown command\n"

#endif
