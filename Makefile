
FILES = src/Channel.cpp \
        src/Client.cpp \
        src/Command.cpp \
        src/Nick.cpp \
        src/Pass.cpp \
        src/Server.cpp \
        src/User.cpp \
        src/Join.cpp \
        src/GptBot.cpp \
        src/Quit.cpp \
        src/Part.cpp \
        src/Topic.cpp \
        src/Invite.cpp \
        src/Kick.cpp \
        src/Privmsg.cpp \
        src/Mode.cpp

# Generazione degli oggetti nella directory src
OBJ = $(FILES:.cpp=.o)
HEADERS = src/ft_irc.h

FLAGS = -g -Wall -Wextra -Werror -std=c++98

NAME = ircserv

GPTSERV=src/gptserv

PID=

$(NAME): src/ft_irc.cpp $(OBJ) $(HEADERS)
	@echo "Compiling $(NAME)..."
	@c++ $(FLAGS) src/ft_irc.cpp $(OBJ) -o $(NAME)
	@./$(GPTSERV) 4433 & 
# Assicurati che i file oggetto siano generati e ricercati nella directory src
%.o: %.cpp
	c++ -c $(FLAGS) $< -o $@

all: $(NAME)

test:
	@c++ -std=c++11 tests/test_main.cpp -o test_irc
	@./test_irc

# Modifica qui per avviare correttamente il server e $(NAME)
stop_server:
	@PID=$$(ps aux | grep '[g]ptserv' | awk '{print $$2}' | head -n 1); \
	if [ -n "$$PID" ]; then \
		kill $$PID; \
	fi

clean: stop_server
	rm -f src/*.o

fclean: clean stop_server
	rm -f $(NAME)

re: fclean all

.PHONY: stop_server all clean fclean re
