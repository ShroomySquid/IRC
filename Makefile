NAME			=	ircserv

SRC_DIRECT		=	src/

MY_FILES		=	main.cpp utils.cpp Client.cpp Channel.cpp Commands/cmd_invite.cpp Commands/Cmd_join.cpp Commands/Cmd_kick.cpp Commands/Cmd_privmsg.cpp Server.cpp Commands/pass.cpp Commands/user.cpp Commands/nick.cpp ServerEventLoop.cpp Commands/Cmd_topic.cpp Commands/Cmd_mode.cpp Commands/Cmd_part.cpp ResponseHandler.cpp Commands/cap.cpp

MY_SOURCES		=	$(addprefix $(SRC_DIRECT), $(MY_FILES))

CC		=	c++

CCFLAGS		=	-Wall -Wextra -Werror -std=c++98

DEBUG_FLAGS	=	-g -DDEBUG

all:		$(NAME)

$(NAME):	$(O_DIRECT) $(MY_SOURCES)
	$(CC) $(CCFLAGS) -o $(NAME) $(MY_SOURCES)

run:	$(O_DIRECT) $(MY_SOURCES)
	$(CC) $(CCFLAGS) -fsanitize=address -g -o $(NAME) $(MY_SOURCES)
	./$(NAME) 6667 patate

debug:	CCFLAGS += $(DEBUG_FLAGS)
debug:	re

clean:
	rm -f *.o
	rm -rf *.dSYM

fclean:		clean
	rm -f $(NAME) test

re:		fclean $(NAME)

.PHONY:	all clean fclean re
