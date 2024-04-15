#include "ft_irc.h"
//MIO BRANCH//

Server::Server() {
    client_number = 0;
    epollfd = -1;
    serv_fd = -1;
    cmds.insert(std::pair<std::string, Command *>("JOIN", new Join()));
    cmds.insert(std::pair<std::string, Command *>("PRIVMSG", new Privmsg(0)));
    cmds.insert(std::pair<std::string, Command *>("MODE", new Mode()));
    cmds.insert(std::pair<std::string, Command *>("NICK", new Nick()));
    cmds.insert(std::pair<std::string, Command *>("PART", new Part()));
    cmds.insert(std::pair<std::string, Command *>("TOPIC", new Topic()));
    cmds.insert(std::pair<std::string, Command *>("INVITE", new Invite()));
    cmds.insert(std::pair<std::string, Command *>("QUIT", new Quit()));
    cmds.insert(std::pair<std::string, Command *>("KICK", new Kick()));
    cmds.insert(std::pair<std::string, Command *>("NOTICE", new Privmsg(1)));
    cmds.insert(std::pair<std::string, Command *>("PASS", new Pass())); //
    cmds.insert(std::pair<std::string, Command *>("USER", new User())); //
    cmds.insert(std::pair<std::string, Command *>("CHATGPT", new GptBot()));
}

void Server::setnonblocking(int &sockfd) {

    // Imposta il socket come non bloccante
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("fcntl failed");
}

Command* Server::Get_cmd(std::string &msg) {
    if (msg.find("PRIVMSG") != std::string::npos)
        return cmds["PRIVMSG"];
    else if (msg.find("NOTICE") != std::string::npos)
        return cmds["NOTICE"];
    else if (msg.find("USERHOST") != std::string::npos)
        return NULL;
    for (std::map<std::string, Command *>::iterator it = cmds.begin(); it != cmds.end(); it++) {
		if (msg.find(it->first) != std::string::npos) {
			return it->second;
		}
	}
	return NULL;
}

void Server::set_socket(char *av1, char *av2) {
    struct sockaddr_in servaddr; // struttura per l'indirizzo e porta del server
    uint16_t port; // porta del server

    port = std::atoi(av1);

    // controllo se porta è valida //inet_addr("192.168.1.1");
    if (port < 1024)
        throw std::runtime_error("invalid port");
    servaddr.sin_family = AF_INET; // ipv4
	servaddr.sin_addr.s_addr = INADDR_ANY; // accetta connessioni da qualsiasi indirizzo
	servaddr.sin_port = htons(port); // porta del server

    if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("socket creation failed");
    int yes = 1;
    if (setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) // permette di riutilizzare la porta del server
        throw std::runtime_error("socket (setsockopt) creation failed");
    if (bind(serv_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) // associa l'indirizzo e la porta al socket del server
        throw std::runtime_error("socket (bind) creation failed");
    if (listen(serv_fd, 0) < 0) // mette il server in ascolto
        throw std::runtime_error("socket (listen) creation failed");

    epollfd = epoll_create(1); // epollfd e' la epoll instance, funge da handle per tutte le operazioni successive
	Server::setnonblocking(serv_fd);
    server_event.events = EPOLLIN; // evento lettura
	server_event.data.fd = serv_fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, serv_fd, &server_event) < 0)
		throw std::runtime_error("epoll_ctl failed");
    pass = av2;
    if (pass.length() == 0)
        throw std::runtime_error("password not set");
}


int Server::Get_fd() {
    return serv_fd;
}

int Server::Get_epollfd() {
    return epollfd;
}


int &Server::Get_client_number() {
    return client_number;
}

std::string Server::Get_pass() {
    return pass;
}

Server::~Server() {
    if (serv_fd > 0)
        close(serv_fd);
    for (std::map<std::string, Command *>::iterator it = cmds.begin(); it != cmds.end(); it++)
        delete it->second;
    if (epollfd > 0)
        close(epollfd);
}

