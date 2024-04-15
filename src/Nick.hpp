#pragma once

#define ERR_NONICKNAMEGIVEN ":ft_irc.network.eu 431 :No nickname given\r\n"
#define ERR_NICKNAMEINUSE(nick) ":ft_irc.network.eu 433 " + nick + " :Nickname is already in use\r\n"
#define ERR_ERRONEUSNICKNAME(nick) ":ft_irc.network.eu 432 " + nick + " :Erroneous nickname\r\n"
//CAPIRE BENE QUESTI

class Nick : public Command {
    private:
        bool check_valid_chars(std::string nickname);
    public:
        Nick();

        void cmd_exec(int &fd);
        void cmd_gen_response(std::string &msg, int fd);
        ~Nick();
};