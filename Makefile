# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jariza-o <jariza-o@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/14 11:54:06 by jariza-o          #+#    #+#              #
#    Updated: 2024/05/14 12:00:38 by jariza-o         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFLAGS	=	-Wall -Werror -Wextra -std=c++98 -I./includes

SRC		=	src/main.cpp \

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

valgrind: all
	@valgrind --leak-check=full ./ircserv 12345 1111

.PHONY: all clean fclean re valgrind