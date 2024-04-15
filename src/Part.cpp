#include "ft_irc.h"
Part::Part() : Command("PART") {}

void Part::cmd_gen_response(std::string &msg, int fd) {
    (void) msg;
    (void) fd;
    response = "";
    if (command[0] != "PART" || command[1] == "") {
        response = ERR_NEEDMOREPARAMS(Get_type()); // forse wrong command
        return;
    }
    Command::parse_command();
}

void Part::cmd_exec(int &fd) {
    if (response != "") {
        send(fd, response.c_str(), response.size(), MSG_NOSIGNAL);
        return;
    }
    Client *client = clients[fd];

    for (std::map<std::string, std::string>::iterator it = cmd_map.begin(); it != cmd_map.end(); it++) {
        if (channels.find(it->first) == channels.end()) {
            response = ERR_NOSUCHCHANNEL(it->first);
        } else if (client->cl_channels.find(it->first) == client->cl_channels.end()) {
            response = ERR_NOTONCHANNEL(it->first);
        } else {
            Command::send_all(*channels[it->first], -1, ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " PART " + it->first + " :\r\n");
            client->cl_channels.erase(it->first);
            if (--channels[it->first]->Get_chusers() == 0)
            {
                delete channels[it->first];
                channels.erase(it->first);
            }
            continue;
        }
        send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
    }
}

Part::~Part() {
}




