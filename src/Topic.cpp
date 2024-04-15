#include "ft_irc.h"

Topic::Topic() : Command("TOPIC") {}

Topic::~Topic() {}

void Topic::cmd_gen_response(std::string &msg, int fd)
{
	(void) msg;
	response = "";
	Command::check_prefix(fd);
	if (response != "") return;
	response = command[0] != "TOPIC" ? ERR_UNKNOWNCOMMAND(Get_type()) : response;
	response = response == "" && command[1] == "" ? ERR_NEEDMOREPARAMS(Get_type()) : response;
	response = response == "" && channels.find(command[1]) == channels.end() ? ERR_NOSUCHCHANNEL(command[1]) : response;
	response = response == "" && clients[fd]->cl_channels.find(command[1]) == clients[fd]->cl_channels.end() ? ERR_NOTONCHANNEL(command[1]) : response;
	response = response == "" && command[2] == "" && channels.find(command[1])->second->Get_chtopic() == "" ? RPL_NOTOPIC(command[1], clients[fd]->Get_nickname()) : response;
}

void Topic::cmd_exec(int &fd)
{
	if (response != "") {
		send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
		return;
	}

	if (command[2] == "") {
		response = RPL_TOPIC(command[1], channels[command[1]]->Get_chtopic(), clients[fd]->Get_nickname());
		send(fd, response.c_str(), response.length(), MSG_NOSIGNAL); 
		return ;
	}
	else if (channels[command[1]]->Get_optopic() && clients[fd]->cl_channels.find(command[1])->second == 0) {
		response = ERR_CHANOPRIVSNEEDED(command[1]);
		send(fd, response.c_str(), response.length(), MSG_NOSIGNAL); 
		return ;
	}
	std::string topic = "";
	if (command[2] == "")
		topic = clients[fd]->Get_nickname();
	else {
		for (int i = 2; command[i] != ""; i++)
			topic += " " + command[i];
		topic.erase(0, 1);
	}
	channels[command[1]]->Set_chtopic(topic.substr(1));
	response = RPL_TOPIC(command[1], channels[command[1]]->Get_chtopic(), clients[fd]->Get_nickname());
	Command::send_all(*channels[command[1]], -1, response);
}