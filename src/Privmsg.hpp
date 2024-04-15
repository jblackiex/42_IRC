#pragma once

#define ERR_NORECIPIENT(nickname, command) ":ft_irc.network.eu 411 " + nickname + " :No recipient given (" + command + ")\r\n"
#define ERR_NOTEXTTOSEND(nickname) ":ft_irc.network.eu 412 " + nickname + " :No text to send\r\n"
#define ERR_CANNOTSENDTOCHAN(channel) ":ft_irc.network.eu 404 " + channel + " :Cannot send to channel\r\n"
#define ERR_TOOMANYTARGETS(nickname) ":ft_irc.network.eu 407 " + nickname + " :Duplicate recipients. No message delivered\r\n"
#define RPL_TEXT(nickname, username, recipient, command, text) ":" + nickname + "!" + username + "@" + "ft_irc.network.eu " + command + " " + recipient + " " + text + "\r\n"
class Privmsg : public Command
{
	public:
		Privmsg(int flag);
		~Privmsg();

		void cmd_gen_response(std::string &msg, int fd);
		void cmd_exec(int &fd);
};