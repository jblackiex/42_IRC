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
        src/Mode.cpp \

# Modifica questa riga per generare gli oggetti nella directory src
OBJ = $(FILES:.cpp=.o)
HEADERS = src/ft_irc.h

FLAGS = -g -Wall -Wextra -Werror -std=c++98

NAME = ircserv

# Modifica il percorso per l'eseguibile qui per lasciarlo fuori dalla src
$(NAME): src/ft_irc.cpp $(OBJ) $(HEADERS)
	c++ $(FLAGS) src/ft_irc.cpp $(OBJ) -o $(NAME)

# Assicurati che i file oggetto siano generati e ricercati nella directory src
%.o: %.cpp %.hpp
	c++ -c $(FLAGS) $< -o $@

all: $(NAME)

clean:
	rm -f src/*.o

fclean: clean
	rm -f $(NAME)

re: fclean all
