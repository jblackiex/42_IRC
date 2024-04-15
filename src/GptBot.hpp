#pragma once

#define GPT_WRCMD(nick) ":ft_irc.network.eu 462 " + nick + " :---> type CHATGPT <message> to start a conversation with GPT\r\n"
#define GPT_ERR(nick) ":ft_irc.network.eu 462 " + nick + " :Error, ChatGPT is not available at the moment, closing connection..\r\n"
#define GPT_OFF(nick) ":ft_irc.network.eu 462 " + nick + " :ChatGPT is now OFF\r\n"
#define GPT_NOT_RUNNING(nick) ":ft_irc.network.eu 462 " + nick + " :ChatGPT is not running\r\n"
#define GPT_CHNG_ROLE(nick) ":ft_irc.network.eu 462 " + nick + " :--> CHATGPT ROLE is set <--\r\n"
#define GPT_WRROLE(nick) ":ft_irc.network.eu 462 " + nick + " :<role> must be > 20 bytes\r\n"
class GptBot : public Command {
    private:
        struct epoll_event gpt_event;
        struct sockaddr_in gpt_addr;
        int tmp_fd;
        char buff[4510];
    public:
        GptBot();

        // void recv_from_gpt(int &fd);
        void cmd_exec(int &fd);
        void cmd_gen_response(std::string &msg, int fd);
        ~GptBot();
};