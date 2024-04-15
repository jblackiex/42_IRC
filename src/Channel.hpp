#ifndef CHANNEL_HPP
#define CHANNEL_HPP

//RPL MESSAGES//
#define RPL_TOPIC(channel, topic, nickname) ":ft_irc.network.eu 332 " + nickname + " " + channel + " :" + topic + "\r\n"
#define RPL_NOTOPIC(channel, nickname) ":ft_irc.network.eu 331 " + nickname + " " + channel + " :No topic is set\r\n"
#define RPL_NAMREPLY(channel, nickname, nicknames) ":ft_irc.network.eu 353 " + nickname + " = " + channel + " :" + nicknames + "\r\n"
#define RPL_ENDOFNAMES(channel, nickname) ":ft_irc.network.eu 366 " + nickname + " " + channel + " :End of NAMES list\r\n"

//ERROR MESSAGES//
#define ERR_NOSUCHCHANNEL(channel) ":ft_irc.network.eu 403 " + channel + " :" + channel + "\r\n"
#define ERR_NOTONCHANNEL(channel) ":ft_irc.network.eu 442 " + channel + " :" + channel + "\r\n"
#define ERR_NOSUCHNICK(nickname) ":ft_irc.network.eu 401 " + nickname + " :" + nickname + "\r\n"
#define ERR_INVITEONLYCHAN(channel) ":ft_irc.network.eu 473 "  + channel + " :Cannot join channel (+i)\r\n"
#define ERR_USERONCHANNEL(nickname, channel) ":ft_irc.network.eu 443 " + nickname + " " + channel + " :is already on channel\r\n"
#define ERR_CHANOPRIVSNEEDED(channel) ":ft_irc.network.eu 482 " + channel + " :" + channel + "\r\n"
#define ERR_USERNOTINCHANNEL(nickname, channel) ":ft_irc.network.eu 441 " + nickname + " " + channel + " :They aren't on that channel\r\n"

class Channel {
    private:
        std::string ch_name; // channel name
        std::string ch_topic; // topic of channel
        std::string ch_pass; // password
        int user_ch_limit; // how many users can join
        int ch_users; // number of users in channel
        int invite_only;
        int op_topic; // if 1 operator can't set topic
    public:
        Channel(const std::string &ch_name);

        std::string &Get_chname();
        std::string &Get_chtopic();
        std::string &Get_chpass();
        int &Get_userchlimit();
        int &Get_chusers();
        int &Get_inviteonly();
        int &Get_optopic();

        void Set_chpass(const std::string &ch_pass);
        void Set_chtopic(const std::string &ch_topic);
        void Set_userchlimit(int user_ch_limit);

        ~Channel();
};

#endif