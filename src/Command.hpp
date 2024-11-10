
#ifndef COMMAND_HPP
#define COMMAND_HPP

//RPL MESSAGES//
#define RPL_WELCOME(nick) ":ft_irc.network.eu 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "\r\n"
#define RPL_YOURHOST(nick) ":ft_irc.network.eu 002 " + nick + " :IRC_chat_V_1.0\r\n"
#define RPL_CREATED(nick) ":ft_irc.network.eu 003 " + nick + " :This server was created on 27/02/2024\r\n"
#define RPL_MOTD(nick) ":ft_irc.network.eu 372 " + nick + " :- Welcome -\r\n"
#define RPL_ENDOFMOTD(nick) ":ft_irc.network.eu 376 " + nick + " :End of MOTD command\r\n"
//ERROR MESSAGES//
#define ERR_NEEDMOREPARAMS(command) ":ft_irc.network.eu 461 " + command + " :" + command + "\r\n"
#define ERR_ALREADYREGISTRED(lastparam) ":ft_irc.network.eu 462 " + lastparam + " :Unauthorized command (already registered)\r\n"
#define ERR_UNKNOWNCOMMAND(command) ":ft_irc.network.eu 421 " + command + " :" + command + "\r\n"
#define ERR_INVALIDPREFIX(prefix) ":ft_irc.network.eu 400 " + prefix + " :Invalid\r\n"

// #define ERR_NEEDMOREPARAMS(command) ":ft_irc.network.eu 461 " + command + " :" + command + "\r\n"
class Channel;

class Command {
    protected:
        std::string type; // es: PASS o JOIN
        std::string command[17]; // comando separato in blocchi // max 17 blocchi (prefix + command + 15max parameters)
        std::string response; // risposta del server
        std::map<std::string, std::string> cmd_map; // per JOIN PRIVMSG e PART
        bool space_checker(std::string &msg);
    public:
        Command(std::string type);

        static Command *cmd_select(std::string &msg, int fd);
        virtual void cmd_gen_response(std::string &msg, int fd) = 0; // ogni Command parsa in modo diverso
        virtual void cmd_exec(int &fd) = 0; // ogni Command esegue in modo diverso 
        void parse_command();
        void check_prefix(int fd);
        void send_all(Channel channel, int fd, std::string msg);
        std::string Get_nicknames(Channel channel);

        void Set_commands(std::string cmdz[17]);
        std::string &Get_type();
        std::string Get_commands(int i);
        virtual ~Command();
};

#endif