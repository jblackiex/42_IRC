#include "ft_irc.h"
Pass::Pass() : Command("PASS") {}

void Pass::cmd_gen_response(std::string &msg, int fd) {
    (void) fd;
    (void) msg;
    response = "";
    if (command[0] != "PASS" || command[1] == "") {
        response = ERR_NEEDMOREPARAMS(Get_type()); // forse wrong command
        return;
    }
    std::string serv_pass = server.Get_pass();
    command[2] != "" || (command[1][0] != ':' && command[2] != "") || command[1].erase(0, 1) != serv_pass ? response = ERR_PASSWDMISMATCH(Get_commands(1)) : response;
}

void Pass::cmd_exec(int &fd) {
    int flag = clients[fd]->Get_flag();
    if (response != "" && flag < 3)
        clients[fd]->Set_disconnect(true);
    if (flag >= 3)
        response = ERR_ALREADYREGISTRED(Get_type());
    send(fd, response.c_str(), response.size(), MSG_NOSIGNAL);
    response != "" ? throw std::runtime_error(Get_type()) : response;
}

Pass::~Pass() {
}




