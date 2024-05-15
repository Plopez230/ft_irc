# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/14 11:54:06 by jariza-o          #+#    #+#              #
#    Updated: 2024/05/15 13:45:37 by jariza-o         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS	=	-Wall -Werror -Wextra -std=c++98 -I./include

SRC		=	src/main.cpp src/channel.cpp src/command.cpp src/server.cpp src/socket_manager.cpp \
			src/user.cpp \

OBJ		=	$(addprefix obj/, $(SRC:.cpp=.o))

NAME	=	ircserv

all:	$(NAME)
$(NAME): ${OBJ}
	@g++ ${CFLAGS} ${OBJ} -o ${NAME}

obj/%.o: %.cpp
	@mkdir -p $(dir $@)
	@g++ ${CFLAGS} -c $< -o $@

clean:
	@rm -rf ${OBJ} obj/

fclean:	clean
	@rm -f ${NAME}

re:		fclean all

r:		fclean all
	@./ircserv 12345 1111

valgrind: all
	@valgrind --leak-check=full ./ircserv 12345 1111

.PHONY: all clean fclean re r valgrind
