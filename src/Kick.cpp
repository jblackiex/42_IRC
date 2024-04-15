#include "ft_irc.h"

Kick::Kick() : Command("KICK") {}

Kick::~Kick() {}

void Kick::cmd_gen_response(std::string &msg, int fd)
{
	(void) msg;
	response = "";
	Command::check_prefix(fd);
	if (response != "") return;
	response = command[2] == "" ? ERR_NEEDMOREPARAMS(Get_type()) : response;
	response = response == "" && command[0] != "KICK" ? ERR_UNKNOWNCOMMAND(Get_type()) : response;
	response = response == "" && cl_db.find(command[2]) == cl_db.end() ? ERR_NOSUCHNICK(command[2]) : response;
	response = response == "" && channels.find(command[1]) == channels.end() ? ERR_NOSUCHCHANNEL(command[1]) : response;
	response = response == "" && clients[fd]->cl_channels.find(command[1]) == clients[fd]->cl_channels.end() ? ERR_NOTONCHANNEL(command[1]) : response;
	response = response == "" && clients[fd]->cl_channels.find(command[1])->second == 0 ? ERR_CHANOPRIVSNEEDED(command[1]) : response;
	response = response == "" && clients[cl_db[command[2]]]->cl_channels.find(command[1]) == clients[cl_db[command[2]]]->cl_channels.end() ? ERR_USERNOTINCHANNEL(command[2], command[1]) : response;
}

void Kick::cmd_exec(int &fd)
{
	if (response != "") {
		send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
		return;
	}
	int fd_kick = cl_db[command[2]];
	std::string comment = "";
	if (command[3] == "")
		comment = clients[fd]->Get_nickname();
	else {
		for (int i = 3; command[i] != ""; i++)
			comment += " " + command[i];
		comment.erase(0, 1);
	}
	response = ":" + clients[fd]->Get_nickname() + "!" + clients[fd]->Get_username() + "@" + "ft_irc.network.eu" + " KICK " + command[1] + " " + command[2] + " " + comment + "\r\n";
	Command::send_all(*channels[command[1]], -1, response);
	clients[fd_kick]->cl_channels.erase(command[1]);
}