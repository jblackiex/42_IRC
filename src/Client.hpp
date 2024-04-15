
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
class Client {
    private:
        struct epoll_event client_event;
        std::string username; // client username
        std::string nickname; // client nickname
        std::string msg;
        int gpt_fd;
        int client_fd;
        int flag;
        int num_channels; // max should be 10 channels for user
        char buff[102];
        bool disconnect;
    public:
        Client(int &epollfd, int server_fd);
        void cmd_handler();

        std::map<std::string, int> cl_channels; // list of user channels and if user is ch_operator
        std::vector<std::string> invites;

        std::string &Get_username();
        std::string &Get_nickname();
        std::string &Get_msg();
        void Set_disconnect(bool disconnect);
        bool Get_disconnect();
        int &Get_flag();
        int Get_gptfd();
        int Get_num_channels();
        int Get_checkamop(std::string &ch_name);
        int Get_fd();
        void Set_nickname(std::string &nick);
        void Set_username(std::string &user);
        void Set_gptfd(int fd);

        void remove_gptfd();
        ~Client();
};

#endif