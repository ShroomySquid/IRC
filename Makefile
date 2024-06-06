NAME            =   ircserv
SRC_DIRECT      =   src
CMD_DIRECT      =   $(SRC_DIRECT)/Commands
MY_SOURCES      =   $(SRC_DIRECT)/main.cpp \
                    $(SRC_DIRECT)/Client.cpp \
                    $(SRC_DIRECT)/Server.cpp \
                    $(SRC_DIRECT)/ServerEventLoop.cpp \
                    $(SRC_DIRECT)/ResponseHandler.cpp \
                    $(SRC_DIRECT)/Channel.cpp \
                    $(SRC_DIRECT)/utils.cpp \
                    $(CMD_DIRECT)/cap.cpp \
                    $(CMD_DIRECT)/invite.cpp \
                    $(CMD_DIRECT)/join.cpp \
                    $(CMD_DIRECT)/kick.cpp \
                    $(CMD_DIRECT)/mode.cpp \
                    $(CMD_DIRECT)/nick.cpp \
                    $(CMD_DIRECT)/part.cpp \
                    $(CMD_DIRECT)/pass.cpp \
                    $(CMD_DIRECT)/ping.cpp \
                    $(CMD_DIRECT)/privmsg.cpp \
                    $(CMD_DIRECT)/quit.cpp \
                    $(CMD_DIRECT)/topic.cpp \
                    $(CMD_DIRECT)/user.cpp
CC              =   c++
CCFLAGS         =   -Wall -Wextra -Werror -std=c++98 -g
DEBUG_FLAGS     =   -fsanitize=address -g -DDEBUG

all: $(NAME)

$(NAME): $(MY_SOURCES)
	@$(CC) $(CCFLAGS) -o $(NAME) $(MY_SOURCES)

run: $(NAME)
	./$(NAME) 6667 patate

debug: CCFLAGS += $(DEBUG_FLAGS)
debug: re run

kill:
	lsof -i :6667 | awk 'NR > 1 {print $$2}' | xargs kill -9

test: all
	./tests.sh

clean:
	@rm -f *.o
	@rm -rf *.dSYM

fclean: clean
	@rm -f $(NAME) test

tclean:
	rm -f irc_commands.log server.log user1_commands.log user2_commands.log

re: fclean $(NAME)

.PHONY: all clean fclean re
