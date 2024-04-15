#pragma once

#define ERR_BADCHANNELKEY(channel) ":ft_irc.network.eu 475 " + channel + " :" + channel + "\r\n";
#define ERR_UNKNOWNMODE(nick, command, channel) ":ft_irc.network.eu 472 " + nick + " " + command + " :is unknown mode char for " + channel + "\r\n";
class Mode : public Command
{
	public:
		Mode();
		~Mode();

		void cmd_gen_response(std::string &msg, int fd);
		void cmd_exec(int &fd);
};