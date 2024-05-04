#include "ft_irc.h"
GptBot::GptBot() : Command("CHATGPT") {
    bzero(&gpt_addr, sizeof(gpt_addr));
    bzero(&gpt_event, sizeof(gpt_event));
    gpt_addr.sin_family = AF_INET;
    gpt_addr.sin_port = htons(4433);
    gpt_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    tmp_fd = -1;
}

void GptBot::cmd_gen_response(std::string &msg, int fd) {
    (void) fd;
    response = "";
    if (msg == "gpt") {
        tmp_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (tmp_fd == -1) {
            std::cerr << "Could not create socket gpt" << std::endl;
        }
        if (connect(tmp_fd, (struct sockaddr *)&gpt_addr, sizeof(gpt_addr)) < 0) {
            std::cerr << "Connection Failed" << std::endl;
        }
        return;
    }
    tmp_fd = clients[fd]->Get_gptfd();
    if (tmp_fd == -1 || clients.find(gpt_db[tmp_fd]) == clients.end()){
        std::string off = "CHATGPT OFF";
        std::cout << "OHCHEE" << std::endl;
        response = GPT_WRCMD(off);
        return;
    }
    std::string nick = clients[gpt_db[tmp_fd]]->Get_nickname();
    if (command[0] != "CHATGPT" || command[1] == "")
        response = GPT_WRCMD(nick);
    else if (command[0] == "CHATGPT" && command[1] == "OFF" && command[2] == "")
        response = tmp_fd > 0 ? GPT_OFF(nick) : GPT_NOT_RUNNING(nick);
    else if (command[0] == "CHATGPT" && command[1] == "ROLE")
        response = msg.length() > 30 ? GPT_CHNG_ROLE(nick) : GPT_WRROLE(nick);
}

void GptBot::cmd_exec(int &fd) {
    Client *client = clients.find(fd)->second;
    if (!client)
        return;
    std::string msg = client->Get_msg();
    int ret_recv;
    bzero(buff, 4510);

    if (client->Get_gptfd() < 0 && response == "") {
        client->Set_gptfd(tmp_fd);
        gpt_event.events = EPOLLIN;
        gpt_event.data.fd = tmp_fd;
        Server::setnonblocking(tmp_fd);
        epoll_ctl(server.Get_epollfd(), EPOLL_CTL_ADD, tmp_fd, &gpt_event);
        gpt_db.insert(std::pair<int, int>(tmp_fd, fd));
        response = "NICK " + client->Get_nickname() + "\r\n";
        send(tmp_fd, response.c_str(), response.length(), MSG_NOSIGNAL);
        response = "";
        return;
    }
    else if (response != "") { // vengo da Command.cpp e ho ottenuto errore
        int flag = 0;
        if (response.find("OFF") != std::string::npos) {
        if (client->Get_gptfd() > 0 && send(client->Get_gptfd(), msg.c_str(), msg.length(), MSG_NOSIGNAL) < 0) {
            client->remove_gptfd();
            response = GPT_ERR(client->Get_nickname());
            send(fd, response.c_str(), response.length(), MSG_NOSIGNAL); 
         }
            client->remove_gptfd();
            tmp_fd = -1;
        }
        else if (response.find("CHATGPT ROLE") != std::string::npos)
            ++flag;
        send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
        // response = "";
        if (!flag)
            return;
    }

    if (send(client->Get_gptfd(), "", 0, MSG_NOSIGNAL) < 0) {
        client->remove_gptfd();
        response = GPT_ERR(client->Get_nickname());
        send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
        return;
    }

    ret_recv = recv(client->Get_gptfd(), buff, 4500, 0);
    if (ret_recv <= 0) { // significa che non vengo da ft_irc ma da Command.cpp quindi non e' server_gpt che ha inviato ma sono io che invio a lui
        msg = "MAX 4500 caratteri: " + msg;
         if (send(client->Get_gptfd(), msg.c_str(), msg.length(), MSG_NOSIGNAL) < 0) {
            client->remove_gptfd();
            response = GPT_ERR(client->Get_nickname());
            send(fd, response.c_str(), response.length(), MSG_NOSIGNAL); 
         }
        return;
    }
    buff[ret_recv] = '\0';
    std::string gpt_msg(buff);
    send(fd, gpt_msg.c_str(), gpt_msg.length(), MSG_NOSIGNAL);
}

GptBot::~GptBot() {
}




