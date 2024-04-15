#include "ft_irc.h"

User::User() : Command("USER") {}

void User::cmd_gen_response(std::string &msg, int fd) {
    (void) fd;
    (void) msg;
    response = "";
    command[0] != "USER" ? response = ERR_UNKNOWNCOMMAND(Get_type()) : response = ""; // or wrong command
    response == "" && (command[4] == "" || (command[1] == "" || command[2] == "" || command[3] == "")) ? response = ERR_NEEDMOREPARAMS(Get_type()) : response = "";
    response != "" || command[4][0] != ':' ? response = ERR_NEEDMOREPARAMS(Get_type()) : response = ""; // or wrong command
}

void User::cmd_exec(int &fd) {
    Client *client = clients[fd];
    int &flag = client->Get_flag();
    if (response != "" || flag > 2) {
        flag > 2 ? response = ERR_ALREADYREGISTRED(Get_type()) : response;
        send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
        throw std::runtime_error(Get_type());
    }
    if (flag == 2) {
    std::string nick = client->Get_nickname();
        std::string welcome[5] = {
            RPL_WELCOME(nick),
            RPL_YOURHOST(nick),
            RPL_CREATED(nick),
            RPL_MOTD(nick),
            RPL_ENDOFMOTD(nick)
        };
        send(fd, welcome[0].c_str(), welcome[0].length(), MSG_NOSIGNAL);
        send(fd, welcome[1].c_str(), welcome[1].length(), MSG_NOSIGNAL);
        send(fd, welcome[2].c_str(), welcome[2].length(), MSG_NOSIGNAL);
        send(fd, welcome[3].c_str(), welcome[3].length(), MSG_NOSIGNAL);
        send(fd, welcome[4].c_str(), welcome[4].length(), MSG_NOSIGNAL);
    }
    client->Set_username(command[1]);
}

User::~User() {
}




