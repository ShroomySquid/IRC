NAME			=	ircserv

SRC_DIRECT		=	src/

MY_FILES		=	main.cpp utils.cpp Client.cpp Channel.cpp Commands/Cmd_join.cpp Commands/Cmd_kick.cpp Commands/Cmd_privmsg.cpp Server.cpp Commands/pass.cpp Commands/user.cpp Commands/nick.cpp ServerEventLoop.cpp Commands/Cmd_topic.cpp Commands/Cmd_mode.cpp

TEST_FILES		=	test_main.cpp utils.cpp

MY_SOURCES		=	$(addprefix $(SRC_DIRECT), $(MY_FILES))

TEST_SOURCES	=	$(addprefix $(SRC_DIRECT), $(TEST_FILES))

CC		=	c++

CCFLAGS		=	-Wall -Wextra -Werror -std=c++98

DEBUG_FLAGS	=	-g -DDEBUG

all:		$(NAME)

$(NAME):	$(O_DIRECT) $(MY_SOURCES)
	$(CC) $(CCFLAGS) -o $(NAME) $(MY_SOURCES)

run:	$(O_DIRECT) $(MY_SOURCES)
	$(CC) $(CCFLAGS) -o $(NAME) $(MY_SOURCES)
	./$(NAME) 6667 patate

test:	$(O_DIRECT) $(TEST_SOURCES)
	$(CC) $(CCFLAGS) -o test $(TEST_SOURCES)

debug:	CCFLAGS += $(DEBUG_FLAGS)
debug:	re

clean:
	rm -f *.o
	rm -rf *.dSYM

fclean:		clean
	rm -f $(NAME) test

re:		fclean $(NAME)

.PHONY:	all clean fclean re
