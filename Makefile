NAME            =   ircserv
SRC_DIRECT      =   src
MY_SOURCES      =   $(shell find $(SRC_DIRECT) -name "*.cpp")
CC              =   c++
CCFLAGS         =   -Wall -Wextra -Werror -std=c++98 -fsanitize=address
DEBUG_FLAGS     =   -g -DDEBUG

all: $(NAME)

$(NAME): $(MY_SOURCES)
	$(CC) $(CCFLAGS) -o $(NAME) $(MY_SOURCES)

run: $(MY_SOURCES)
	$(CC) $(CCFLAGS) -g -fsanitize=address -o $(NAME) $(MY_SOURCES)
	./$(NAME) 6667 patate


debug: CCFLAGS += $(DEBUG_FLAGS)
debug: re run

clean:
	rm -f *.o
	rm -rf *.dSYM

fclean:         clean
	rm -f $(NAME) test

re:             fclean $(NAME)

.PHONY: all clean fclean re
