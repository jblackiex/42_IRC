#include "ft_irc.h"
Quit::Quit() : Command("QUIT") {}

void Quit::cmd_gen_response(std::string &msg, int fd) {
    (void) fd;
    (void) msg;
    response = "";
    if (command[0] != "QUIT") {
        response = ERR_NEEDMOREPARAMS(Get_type()); // forse wrong command
        return;
    }
    command[2] != "" && command[1][0] != ':' ? response = ERR_UNKNOWNCOMMAND(command[0]) : response;
}

void Quit::cmd_exec(int &fd) {
    Client *client = clients[fd];
    if (response != "")
        send(fd, response.c_str(), response.size(), MSG_NOSIGNAL);
    else {
    // invio a tutti i canali in cui è presente l'utente che si è disconnesso
        command[1] == "" ? command[1] = "Client Quit in a weird way" : command[1];
        std::string quit_msg = ":" + clients[fd]->Get_nickname() + " QUIT :" + Get_commands(1) + "\r\n";

        for (std::map<std::string, int>::iterator it = client->cl_channels.begin(); it != client->cl_channels.end(); it++) {
            Channel *channel = channels[it->first];
            Command::send_all(*channel, fd, quit_msg);
             if (--channel->Get_chusers() == 0) {
                delete channel;
                channels.erase(it->first);
            }
        }
    }
}

Quit::~Quit() {
}




