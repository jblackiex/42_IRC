#pragma once

#include <string.h>
#include <stdexcept>
#include <sstream>
#include <string>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <iostream>
#include <netinet/in.h>
#include <map>
#include <vector>
#include <algorithm>
#include <signal.h>


#define MAX_EVENTS 100 // capire  come influisce il numero massimo di connessioni simultanee

#include "Command.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Pass.hpp"
#include "Nick.hpp"
#include "User.hpp"
#include "GptBot.hpp"
#include "Quit.hpp"
#include "Join.hpp"
#include "Part.hpp"
#include "Topic.hpp"
#include "Invite.hpp"
#include "Kick.hpp"
#include "Privmsg.hpp"
#include "Mode.hpp"

extern std::map<std::string, int> cl_db; // from nickname to fd
extern std::map<int, int> gpt_db; // from gpt_fd to fd del client
extern std::map<std::string, Channel *> channels; // list of channels with name/settings
extern std::map<int, Client *> clients; // from fd to Client (fd, nick, username, etc.)
extern Server server;

size_t	ft_strlen(const char *s);