# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/14 11:54:06 by jariza-o          #+#    #+#              #
#    Updated: 2024/05/28 13:01:19 by jariza-o         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS	=	-Wall -Werror -Wextra -std=c++98 -I./include

SRC		=	src/main.cpp src/channel.cpp src/command.cpp src/server.cpp \
			src/socket_manager.cpp src/user.cpp src/commands/nick.cpp \
			src/replies.cpp src/commands/join.cpp src/commands/mode.cpp \
			src/commands/pass.cpp src/commands/user.cpp src/validation.cpp \
			src/tracer.cpp src/commands/quit.cpp src/commands/privmsg.cpp \
			src/commands/part.cpp src/commands/kick.cpp \
			src/commands/invite.cpp src/commands/topic.cpp

SRC_BONUS	=	src_bonus/bot.cpp src_bonus/main.cpp

OBJ		=	$(addprefix obj/, $(SRC:.cpp=.o))

OBJ_BONUS	=	$(addprefix obj/, $(SRC_BONUS:.cpp=.o))

NAME	=	ircserv

NAME_BONUS	=	bot

all:	$(NAME)
$(NAME): ${OBJ}
	@clang++ ${CFLAGS} ${OBJ} -o ${NAME}

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	@clang++ ${CFLAGS} -c $< -o $@

clean:
	@rm -rf ${OBJ} ${OBJ_bonus} obj/

fclean:	clean
	@rm -f ${NAME} ${NAME_BONUS}

fclean_bonus:	clean
	@rm -f ${NAME_BONUS}

re:		fclean all

r:		fclean all
	@./ircserv 12345 1111

bonus:	$(NAME_BONUS)
$(NAME_BONUS): ${OBJ_BONUS}
	@clang++ ${CFLAGS} ${OBJ_BONUS} -o ${NAME_BONUS}

re_bonus:	fclean_bonus bonus


valgrind: all
	@valgrind --leak-check=full ./ircserv 12345 1111

.PHONY: all clean fclean fclean_bonus re r valgrind bonus re_bonus
