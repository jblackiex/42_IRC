#pragma once

#define ERR_BADCHANNELKEY(channel) ":ft_irc.network.eu 475 " + channel + " :" + channel + "\r\n";
#define ERR_CHANNELISFULL(channel) ":ft_irc.network.eu 471 " + channel + " :Cannot join channel (+l)\r\n";
class Join : public Command
{
	public:
		Join();
		~Join();

		void cmd_gen_response(std::string &msg, int fd);
		void cmd_exec(int &fd);
};