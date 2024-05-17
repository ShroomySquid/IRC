# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbouchar <BrunoPierreBouchard@hotmail.c    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/30 09:59:24 by fbarrett          #+#    #+#              #
#    Updated: 2024/05/17 10:55:42 by fbarrett         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	ircserv

SRC_DIRECT		=	src/

MY_FILES		=	main.cpp utils.cpp Client.cpp Channel.cpp Commands/Cmd_join.cpp Commands/Cmd_kick.cpp Commands/Cmd_privmsg.cpp registration.cpp message_handling.cpp

TEST_FILES		=	test_main.cpp utils.cpp

MY_SOURCES		=	$(addprefix $(SRC_DIRECT), $(MY_FILES))

TEST_SOURCES	=	$(addprefix $(SRC_DIRECT), $(TEST_FILES))

CC		=	c++

CCFLAGS		=	-Wall -Wextra -Werror -std=c++98 -g

all:		$(NAME)

$(NAME):	$(O_DIRECT) $(MY_SOURCES)
	@make test
	$(CC) $(CCFLAGS) -o $(NAME) $(MY_SOURCES)

test:	$(O_DIRECT) $(TEST_SOURCES)
	$(CC) $(CCFLAGS) -o test $(TEST_SOURCES)

clean:
	rm -f *.o
	rm -rf *.dSYM
	
fclean:		clean
	rm -f $(NAME) test

re:		fclean $(NAME)

.PHONY:	all clean fclean re
