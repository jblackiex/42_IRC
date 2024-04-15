#include "ft_irc.h"

Mode::Mode() : Command("MODE") {}

Mode::~Mode() {}

void Mode::cmd_gen_response(std::string &msg, int fd)
{
	(void) msg;
	response = "";
	Command::check_prefix(fd);
	if (response != "") return;
	response = command[0] != "MODE" ? ERR_UNKNOWNCOMMAND(Get_type()) : response;
	response = response == "" && command[1].empty() ? ERR_NEEDMOREPARAMS(Get_type()) : response;
	response = response == "" && channels.find(command[1]) == channels.end() ? ERR_NOSUCHCHANNEL(command[1]) : response;
	response = response == "" && clients[fd]->cl_channels.find(command[1]) == clients[fd]->cl_channels.end() ? ERR_NOTONCHANNEL(command[1]) : response;
	if (response == "" && (command[2] == "" || command[2] == "+b")) {
		response = "exit";
		return ;
	}
	response = response == "" && clients[fd]->cl_channels.find(command[1])->second == 0 ? ERR_CHANOPRIVSNEEDED(command[1]) : response;
}

void Mode::cmd_exec(int &fd)
{
	if (response != "") {
		if (response != "exit")
			send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
		return;
	}
	if (command[2][0] != '+' && command[2][0] != '-')
		command[2] = "+" + command[2];
	char sign = command[2][0];
	command[2].erase(0, 1);
	int len = command[2].length();
	int flag = 3;
	Client *client = clients[fd];
	for (int i = 0; i < len; i++) {
		response = "";
		switch (command[2][i]) {
			case 'i':
				if (sign == '+')
					channels[command[1]]->Get_inviteonly() = 1;
				else
					channels[command[1]]->Get_inviteonly() = 0;
			send_all(*channels[command[1]], -1, ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " MODE " + command[1] + " " + sign + command[2][i] + "\r\n");
				break;
			case 't':
				if (sign == '+')
					channels[command[1]]->Get_optopic() = 1;
				else
					channels[command[1]]->Get_optopic() = 0;
			send_all(*channels[command[1]], -1, ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " MODE " + command[1] + " " + sign + command[2][i] + "\r\n");
				break;
			case 'k':
				if (sign == '+') {
					if (command[flag] == "") {
						response = ERR_NEEDMOREPARAMS(Get_type());
					}
					else {
						channels[command[1]]->Set_chpass(command[flag]);
						send_all(*channels[command[1]], -1, ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " MODE " + command[1] + " " + sign + command[2][i] + " " + command[flag] + "\r\n");
						flag++;
					}
				}
				else {
					channels[command[1]]->Set_chpass("");
					send_all(*channels[command[1]], -1, ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " MODE " + command[1] + " " + sign + command[2][i] + "\r\n");
				}
				break;
			case 'l':
				if (sign == '+') {
					if (command[flag] == "") {
						response = ERR_NEEDMOREPARAMS(Get_type());
					}
					else {
						int limit = atoi(command[flag].c_str());
						if (limit > 0) {
						channels[command[1]]->Set_userchlimit(limit);
						send_all(*channels[command[1]], -1, ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " MODE " + command[1] + " " + sign + command[2][i] + " " + command[flag] + "\r\n");
						flag++;
						}
					}
				}
				else {
					channels[command[1]]->Set_userchlimit(10);
					send_all(*channels[command[1]], -1, ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " MODE " + command[1] + " " + sign + command[2][i] + "\r\n");
				}
				break;
			case 'o':
				if (command[flag] == "") {
					response = ERR_NEEDMOREPARAMS(Get_type());
					break;
				}
				if (sign == '+') {
					if (cl_db.find(command[flag]) == cl_db.end())
						response = ERR_NOSUCHNICK(command[flag]);
					else if (clients[cl_db[command[flag]]]->cl_channels.find(command[1]) == clients[cl_db[command[flag]]]->cl_channels.end())
							response = ERR_NOTONCHANNEL(command[1]);
					else  {
						clients[cl_db[command[flag]]]->cl_channels.find(command[1])->second = 1;
						client->cl_channels.find(command[1])->second = 1;
						send_all(*channels[command[1]], -1, ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " MODE " + command[1] + " " + sign + command[2][i] + " " + command[flag] + "\r\n");
					}
				}
				else {
					clients[cl_db[command[flag]]]->cl_channels.find(command[1])->second = 0;
					send_all(*channels[command[1]], -1, ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " MODE " + command[1] + " " + sign + command[2][i] + " " + command[flag] + "\r\n");
				}
				flag++;
				break;
			default:
				response = ERR_UNKNOWNMODE(clients[fd]->Get_nickname(), command[2], command[1]);
				break;
		}
		if (response != "")
			send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
	}
}