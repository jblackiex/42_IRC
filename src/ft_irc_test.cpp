#include "ft_irc.h"
// valgrind --leak-check=full --track-fds=yes ./ircserv 8787 banana
// controlla ^[[A^[[ caratteri di escape e "NICK USER"
std::map<std::string, int> cl_db; // from nickname to fd
std::map<int, int> gpt_db; // from gpt_fd to fd del client
std::map<std::string, Channel *> channels; // list of channels with name/settings
std::map<int, Client *> clients; // from fd to Client (fd, nick, username, etc.)
Server server;
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

void sig_handler(int signal)
{
    printf("Signal %d received\n", signal);
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
        delete it->second;
    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        delete it->second;
    for (std::map<int, int>::iterator it = gpt_db.begin(); it != gpt_db.end(); it++)
        close(it->first);
    exit(0);
}

void server_loop()
{
    struct epoll_event events[MAX_EVENTS]; // eventi da monitorare epoll
    int num_fds; // numero di file descriptors pronti
    int current_fd;
    int client_fd;
    int bot = 0;
    int epollfd = server.Get_epollfd();
    int serv_fd = server.Get_fd();

    signal(SIGINT, sig_handler);
    signal(SIGTSTP, sig_handler);
    while(1)
    {
        num_fds = epoll_wait(epollfd, events, MAX_EVENTS, -1); // con -1 attende infinitamente fin quando non si verifica un evento
        for (int i = 0; i < num_fds; i++)
        {
            if (events[i].data.fd == serv_fd)
            {
                try {
                    Client *client = new Client(epollfd, serv_fd);
                    client_fd = client->Get_fd();
                    clients.insert(std::pair<int, Client *>(client_fd, client));
                    ++server.Get_client_number();
                }
                catch (const std::exception& e) {
                    clients.erase(client_fd);
                }
                printf("There are %d clients\n", server.Get_client_number());
                break ;
            }
            else
            {
                current_fd = events[i].data.fd;
                if (clients.find(current_fd) == clients.end() && ++bot) // per trovare fd di chat_gpt
                    current_fd = gpt_db[current_fd]; // forse flag e skippare
                try {
                    Client *tmp = clients[current_fd]; // clients[client_fd
                    if (bot) {
                        Command *cmd = server.cmds["CHATGPT"];
                        cmd->cmd_exec(current_fd);
                        bot = 0;
                    }
                    else
                        tmp->cmd_handler();
                    if (tmp->Get_disconnect()) {
                        std::cout << "Client " << tmp->Get_nickname() << " disconnected" << std::endl;
                        delete tmp;
                        clients.erase(current_fd);
                        printf("There are %d clients\n", --server.Get_client_number());
                    }
                }
                catch (std::exception& e) {
                    clients[current_fd]->remove_gptfd();
                    delete clients[current_fd];
                    clients.erase(current_fd);
                    printf("There are %d clients\n", --server.Get_client_number());
                }
            }
        }
    }
}

// int main(int ac, char **av)
// {
//     if (ac != 3)
//     {
//         write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
//         exit(1);
//     }

//     try {
//         server.set_socket(av[1], av[2]);
//     }
//     catch (std::exception &e) {
//         write(2, e.what(), strlen(e.what()));
//         exit(1);
//     }
//     server_loop();
//     return (0);
// }

