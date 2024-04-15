#include "ft_irc.h"
Client::Client(int &epollfd, int server_fd) {
    bzero(&client_event, sizeof(client_event));
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int serv_fd = server_fd; // server.Get_fd();

    if ((client_fd = accept(serv_fd, (struct sockaddr *)&clientaddr, &len)) < 0)
        throw std::exception();
    client_event.data.fd = client_fd;
    client_event.events = EPOLLIN; // MODalita Edge-Triggered vs. Level-Triggered

    if (send(client_fd, "", 0, MSG_NOSIGNAL) < 0) // per client che chiude la connessione in modo inatteso e MSG_NOSIGNAL per evitare che il server crashi
	{
		close(client_fd);
		throw std::exception();
	}

    Server::setnonblocking(client_fd);
    epoll_ctl(epollfd, EPOLL_CTL_ADD, client_fd, &client_event);
    bzero(buff, 102);
    msg = "";
    num_channels = 0;
    disconnect = false;
    username = "";
    nickname = "";
    gpt_fd = -1;
    flag = 0;
}

void Client::cmd_handler() {
    Command *cmd = NULL;
    static int ret_recv;

    if (ret_recv >= 0)
        msg.clear();
    bzero(buff, 102);
    while (!buff[0] || buff[ft_strlen(buff) - 1] != '\n') {
        ret_recv = recv(client_fd, buff, 100, 0);
        if (!ret_recv)
            break;
        else if (ret_recv > 0) {
            buff[ret_recv] = '\0';
            msg += buff;
        }
        else {
            // msg += " ";
            bzero(buff, 102);
            break ;
        }
    }
    if (ret_recv == 0) {
        throw std::exception();
    }
    else if (buff[0] && buff[ft_strlen(buff) - 1] == '\n') {
        cmd = Command::cmd_select(msg, this->client_fd); // ritorna puntatore al comando // Command e' un interfaccia ereditata dalle varie classi comando (Join, Mode, etc.)
        if (cmd) {
            try {
                cmd->cmd_exec(this->client_fd); // ogni comando ha un metodo diverso es. Join::cmd_exec
                flag < 3 ? ++flag : flag;
            } catch (std::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
                cmd = NULL;
            }
        }
        if (cmd)
            std::cout << msg;
    }
}

void Client::Set_nickname(std::string &nick) {
    nickname = nick;
}

void Client::Set_username(std::string &user) {
    username = user;
}

void Client::remove_gptfd() {
    if (gpt_fd > 0) {
        epoll_ctl(server.Get_epollfd(), EPOLL_CTL_DEL, gpt_fd, 0);
        gpt_db.erase(gpt_fd);
        close(gpt_fd);
        gpt_fd = -1;
    }
}

int Client::Get_gptfd() {
    return gpt_fd;
}

void Client::Set_disconnect(bool disconnect)
{
    this->disconnect = disconnect;
}

bool Client::Get_disconnect()
{
	return disconnect;
}

int &Client::Get_flag()
{
	return flag;
}

int Client::Get_num_channels() {
    return num_channels;
}

std::string &Client::Get_nickname() {
    return nickname;
}

std::string &Client::Get_username() {
    return username;
}

int Client::Get_fd() {
    return client_fd;
}

std::string &Client::Get_msg() {
    return msg;
}

int Client::Get_checkamop(std::string &ch_name) {
    std::map<std::string, int>::iterator it = this->cl_channels.find(ch_name);
    if (it == cl_channels.end())
        return -1;
    return it->second;
}

void Client::Set_gptfd(int fd) {
    gpt_fd = fd;
}

Client::~Client() {
    epoll_ctl(server.Get_epollfd(), EPOLL_CTL_DEL, client_fd, 0); // rimosso da epollfd
    close(client_fd);
}

