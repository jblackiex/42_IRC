#include "ft_irc.h"
#include "Nick.hpp"

Nick::Nick() : Command("NICK") {}

bool Nick::check_valid_chars(std::string nickname)
{
    std::string valid_chars = "_-[]`^{}";
    for (size_t i = 0; i < nickname.size(); i++)
    {
        if (!isalnum(nickname[i]) && valid_chars.find(nickname[i]) == std::string::npos)
            return false;
    }
    return true;
}

void Nick::cmd_gen_response(std::string &msg, int fd) {
    (void) msg;
    response = "";
    check_prefix(fd);
    response = command[0] != "NICK" ? ERR_UNKNOWNCOMMAND(Get_type()) : response;
    response = response == "" && command[1] == "" ? ERR_NONICKNAMEGIVEN : response;
    response = response == "" && (command[1].size() > 9  || !check_valid_chars(command[1])) ? ERR_ERRONEUSNICKNAME(command[1]) : response;
}

void Nick::cmd_exec(int &fd) {
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
        if (it->first == fd)
            continue;
        if (it->second->Get_nickname() == command[1])
            response = ERR_NICKNAMEINUSE(command[1]);
    }
    Client *client = clients[fd];
    std::string oldnick = client->Get_nickname();
    int &flag = client->Get_flag();
    if (response != "") {
        send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
        throw std::runtime_error(Get_type());
    }
    else {
        std::string nick = command[1] == "NICK" ? command[2] : command[1];
        client->Set_nickname(nick);
        cl_db.insert(std::pair<std::string, int>(nick, fd));
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
    else if (flag == 3) {
        std::string msg = ":" + oldnick + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " NICK :" + client->Get_nickname() + "\r\n";
        for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++) 
            send(it->first, msg.c_str(), msg.length(), MSG_NOSIGNAL);
    }
}

Nick::~Nick() {
}




