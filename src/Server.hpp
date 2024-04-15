#ifndef SERVER_HPP
#define SERVER_HPP

class Server {
    private:
        struct epoll_event server_event; // struttura per l'evento del server
        int serv_fd; // file descriptor del server
        int epollfd; // file descriptor di epoll
        int client_number; // numero di client connessi
        std::string pass;
    public:
        std::map<std::string, Command *> cmds;
        Server();
        void set_socket(char *av1, char *av2);
        static void setnonblocking(int &sockfd);

        int Get_fd();
        int Get_epollfd();
        int &Get_client_number();
        std::string Get_pass();
        Command *Get_cmd(std::string &cmd);
        ~Server();
};

#endif