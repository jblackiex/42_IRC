#pragma once

#define RPL_INVITING(nickname, channel) ":ft_irc.network.eu 341 " + nickname + " " + channel + "\r\n"
#define RPL_AWAY(nickname) ":ft_irc.network.eu 301 " + nickname + " :is away\r\n"

class Invite : public Command
{
	public:
		Invite();
		~Invite();

		void cmd_gen_response(std::string &msg, int fd);
		void cmd_exec(int &fd);
};