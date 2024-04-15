#include "ft_irc.h"

Invite::Invite() : Command("INVITE") {}

Invite::~Invite() {}

void Invite::cmd_gen_response(std::string &msg, int fd)
{
	(void) msg;
	response = "";
	Command::check_prefix(fd);
	if (response != "") return;
	response = command[2] == "" ? ERR_NEEDMOREPARAMS(Get_type()) : response;
	response = response == "" && (command[0] != "INVITE" || command[3] != "") ? ERR_UNKNOWNCOMMAND(Get_type()) : response;
	response = response == "" && cl_db.find(command[1]) == cl_db.end() ? ERR_NOSUCHNICK(command[1]) : response;
	response = response == "" && channels.find(command[2]) == channels.end() ? ERR_NOSUCHCHANNEL(command[2]) : response;
	response = response == "" && clients[fd]->cl_channels.find(command[2]) == clients[fd]->cl_channels.end() ? ERR_NOTONCHANNEL(command[2]) : response;
	response = response == "" && clients[fd]->cl_channels.find(command[2])->second == 0 ? ERR_CHANOPRIVSNEEDED(command[2]) : response;
	response = response == "" && clients[cl_db[command[1]]]->cl_channels.find(command[2]) != clients[cl_db[command[1]]]->cl_channels.end() ? ERR_USERONCHANNEL(command[1], command[2]) : response;
}

void Invite::cmd_exec(int &fd)
{
	if (response != "") {
		send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
		return;
	}

	response = RPL_INVITING(command[1], command[2]); // FARE RPL_inviting e AWAY

	send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
	int fd_invite = cl_db[command[1]];
	response = ":" + clients[fd]->Get_nickname() + " INVITE " + command[1] + " " + command[2] + "\r\n";
	send(fd_invite, response.c_str(), response.size(), MSG_NOSIGNAL);
	Command::send_all(*channels[command[2]], fd, response);
	if (std::find(clients[fd_invite]->invites.begin(), clients[fd_invite]->invites.end(), command[2]) == clients[fd_invite]->invites.end())
		clients[fd_invite]->invites.push_back(command[2]);
}