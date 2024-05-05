#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <iostream>
#include <map>

#define MAX_EVENTS 101 // capire  come influisce il numero massimo di connessioni simultanee

struct epoll_event client_event;
int sock_fd, g_id, epollfd = 0;
int client_number = 0;
char str[42*4096], buffer[7000];
std::string role = "You are chatbot, skilled in explaining complex programming concepts with creative flair.";
std::map<int, std::string> clients;

#define GPT_VERS(nick) ":ChatGPT 462 chatgpt 3.5-turbo --> " + nick + "\r\n"
#define GPT_WELCOME(nick) ":ChatGPT 001 " + nick + " :Welcome to CHATGPT_BOT weirdo " + nick + "\r\n"
#define GPT_USAGE(nick) ":ChatGPT 002 " + nick + " :Usage -> CHATGPT <message>\r\n"

std::string gpt_header =
" +-----------------------------------+\n"
" ____ _           _    ____ ____ _____\n"
"--- ____--CHAT---____---GPT---_______\n"
"-- / ___|------|  _ \\---made----|_   _|\n"
"  | |  _--------| |_)_|----by---------| |--nbordoni\n"
"- | |_| |-------|___/------>-->-----| |--lnascari\n"
"- \\____|------|_|---------->-->----|_|--gpaoline\n"
" +-----------------------------------+\n";
// VISIBLE RESTORE
// std::string gpt_header = (
//   "+----------------------------------+\n"
//   "|  ____ _           _    ____ ____ _____   |\n"
//   "| / ___| |__   __ _| |_ / ___|  _ \\_   _| |\n"
//   "|| |   | '_ \\ / _` | __| |  _| |_) || |   |\n"
//   "|| |___| | | | (_| | |_| |_| |  __/ | |   |\n"
//   "| \\____|_| |_|\\__,_|\\__|\\____|_|    |_|   |\n"
//   "|      /\\                                  |\n"
//   "|     ( /   @ @    ()                      |\n"
//   "|      \\\\ __| |__  /                       |\n"
//   "|       \\/   \"   \\/                        |\n"
//   "|           -->made by g(epoll)..nbordoni,  |\n"
//   "|                        lnascari          |\n"
//   "+----------------------------------+\n"
// );
void chat_gpt(std::string &client_msg, std::string &output, int fd) {
    // Definisci la richiesta JSON come una stringa
    std::string tmp_role = "You are chatbot, skilled in explaining complex programming concepts with creative flair.";
    int pipefd[2];
    size_t pos = client_msg.find('\r');
    if (pos != std::string::npos)
        client_msg.erase(pos, 2);
    printf("ENTRATO\n");
    client_msg.erase(client_msg.find("\n"), 1);
    if (client_msg.find("ROLE") != std::string::npos) {
        std::string cursor = "ChatGPT>: ";
        client_msg.erase(0, client_msg.find("ROLE") + 5);
        clients[fd] = client_msg;
        printf("ROLE: %s\n", role.c_str());
        output = "";
        return;
    }
    else if (client_msg.find("RESTORE") != std::string::npos || client_msg.find("OFF") != std::string::npos) {
        role = tmp_role;
        output = "";
        return;
    }
    role = clients[fd];
    std::string content = client_msg.erase(client_msg.find("CHATGPT"), 7);
    std::string data = "{"
                       "\"model\": \"gpt-3.5-turbo\","
                       "\"messages\": ["
                       "{"
                       "\"role\": \"system\","
                       "\"content\": \"" + role + "\""
                       "},"
                       "{"
                       "\"role\": \"user\","
                       "\"content\": \"" + content + "\""
                       "}"
                       "]"
                       "}";
    // Prepara gli argomenti per execve
    char* args[] = {
        (char*)"curl",
        (char*)"https://api.openai.com/v1/chat/completions",
        (char*)"-H", (char*)"Content-Type: application/json",
        (char*)"-H", (char*)"Authorization: Bearer ", // Sostituisci YOUR_API_KEY con la tua chiave API effettiva
        (char*)"-d", (char*)data.c_str(),
        nullptr // L'ultimo elemento deve essere nullptr per indicare la fine dell'array
    };
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }
// 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
    // Esegui il comando curl usando execve
    if (fork() == 0) {
        // Sostituisce l'immagine del processo corrente con il processo curl
        close(pipefd[0]); // close the read end of the pipe
        dup2(pipefd[1], 1); // redirect stdout to the write end of the pipe
        close(pipefd[1]); // close the write end of the pipe

        execvp("curl", args);
        std::cerr << "Errore nell'esecuzione di execvp" << std::endl;
        exit(1);
    } else {
        close(pipefd[1]); // close the write end of the pipe
        ssize_t count;
        while ((count = read(pipefd[0], buffer, sizeof(buffer)-1)) != 0) {
            if (count == -1) {
                if (errno == EINTR) {
                    continue; // Se interrotto da un segnale, riprova
                } else {
                    perror("read");
                    return;
                }
            }

            buffer[count] = '\0';
            output += buffer;
        }

        close(pipefd[0]);
        wait(NULL);
    }
    size_t startPos = output.find("content");
    if (startPos != std::string::npos) {
        // Stampa tutto a partire da "message"
        std::string cursor = "ChatGPT>: ";
        output.erase(0, startPos + 11);
        output.erase(output.find("\""));
        // output.erase(output.find("}") - 1, output.length() - 1);
        output = "\n" + cursor + output + "\r\n";
        std::cout << "CONTENT: " << output << std::endl;
    }
    role = tmp_role;
}
void setnonblocking(int sockfd) {
    int flags;

    // Ottieni le attuali flags del file descriptor
    flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1)
		exit(1);

    // Imposta il socket come non bloccante
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
		exit(1);
}

void server_loop(int ac, char **av) {
    std::string client_msg = "";
    struct epoll_event ev, events[MAX_EVENTS];
    int ret_recv, fd;
    int j;
    if (ac != 2)
    {
        write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
        exit(1);
    }
    struct sockaddr_in servaddr;
    uint16_t port = atoi(av[1]);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);
    std::string nick = "user";
    std::string welcome[3] = {
            GPT_WELCOME(nick),
            GPT_VERS(nick),
            GPT_USAGE(nick)
        };
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        exit(1);
    int yes = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
        exit(1);
    if (bind(sock_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        exit(1);
    if (listen(sock_fd, 0) < 0)
        exit(1);
    bzero(&str, sizeof(str));
    int num_fds, i;

	epollfd = epoll_create(1); // epollfd e' la epoll instance, funge da handle per tutte le operazioni successive
	setnonblocking(sock_fd);
	ev.events = EPOLLIN; // evento lettura
	ev.data.fd = sock_fd;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_fd, &ev) < 0)
		exit(1);
    while(1)
    {
        num_fds = epoll_wait(epollfd, events, MAX_EVENTS, -1); // con -1 attende infinitamente fin quando non si verifica un evento
        for (i = 0, j = 0; i < num_fds; i++)
        {
            bzero(&str, strlen(str));
            client_msg = "";
            if (events[i].data.fd == sock_fd)
            {
                ev.data.fd = accept(sock_fd, NULL, NULL);
                setnonblocking(ev.data.fd);
                epoll_ctl(epollfd, EPOLL_CTL_ADD, ev.data.fd, &ev);
                clients.insert(std::pair<int, std::string>((int)ev.data.fd, role));
                printf("There are %d clients\n", ++client_number);
                break;
            }
            else {
			    ret_recv = 100;
                fd = events[i].data.fd;
			    while (ret_recv == 100 || str[strlen(str) - 1] != '\n') {
				ret_recv = recv(fd, str + strlen(str), 100, 0);
                    if (ret_recv <= 0) {
                        ret_recv = 0;
                        bzero(&str, strlen(str));
					    break ;
                    }
			    }
                if (ret_recv <= 0) {
                    bzero(&str, strlen(str));
                    printf("There are %d clients\n", --client_number);
                    clients.erase(fd);
                    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                    break;
                } else {
                    str[ret_recv] = '\0';
                    client_msg = str;
                    std::string send_client = "";
                    printf("INPUT: %s\n", client_msg.c_str());
                    if (client_msg.find("NICK") != std::string::npos) {
                        nick = client_msg.substr(client_msg.find("NICK") + 5);
                        welcome[0] = GPT_WELCOME(nick);
                        welcome[1] = GPT_VERS(nick);
                        welcome[2] = GPT_USAGE(nick);
                        send(fd, gpt_header.c_str(), gpt_header.length(), MSG_NOSIGNAL);
                        send(fd, welcome[0].c_str() , welcome[0].length(), MSG_NOSIGNAL);
                        send(fd, welcome[1].c_str(), welcome[1].length(), MSG_NOSIGNAL);
                        send(fd, welcome[2].c_str(), welcome[2].length(), MSG_NOSIGNAL);
                        continue;
                    }
                    else if (client_msg.find("CHATGPT") == std::string::npos)
                        continue;
                    chat_gpt(client_msg, send_client, fd);
                    send(fd, send_client.c_str(), send_client.size(), MSG_NOSIGNAL);
                }                
            }
        }
        
    }
}


int main(int ac, char **av) {
    server_loop(ac, av);
    return 0;
}

