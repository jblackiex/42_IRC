#include "ft_irc.h"
#include "Privmsg.hpp"

Privmsg::Privmsg(int flag) : Command("PRIVMSG") {
	if (flag)
		this->type = "NOTICE";
}

Privmsg::~Privmsg() {}

void Privmsg::cmd_gen_response(std::string &msg, int fd)
{
	response = "";
	Command::check_prefix(fd);
	if (response != "") return;
	response = command[0] != "PRIVMSG" && command[0] != "NOTICE" ? ERR_UNKNOWNCOMMAND(Get_type()) : response;
	response = response == "" && command[1].empty() ? ERR_NORECIPIENT(clients[fd]->Get_nickname(), Get_type()) : response;
	response = response == "" && command[2].empty() ? ERR_NOTEXTTOSEND(clients[fd]->Get_nickname()) : response;
	if (response == "" && command[2].find("CHATGPT") != std::string::npos) {
		msg.erase(0, msg.find("CHATGPT"));
		if (clients[fd]->Get_gptfd() < 0 && msg.length() < 10)
			msg = "gpt";
		if(msg != "gpt") {
			command[0] = "CHATGPT";
			command[1] = "OFF";
			if(msg.find("OFF") == std::string::npos)
				command[1] = clients[fd]->Get_gptfd() < 0 ? "OFF" : (msg.find("ROLE") != std::string::npos ? "ROLE" : "feat.");
			command[2] = "";
		}
		Command *cmds = server.cmds["CHATGPT"];
		cmds->Set_commands(command);
		cmds->cmd_gen_response(msg, fd);
		cmds->cmd_exec(fd);
		response = "exit";
		return;
	}
	parse_command();
}

// SE NON CI SONO PIU UTENTI NEL CANALE IL CANALE VIENE ELIMINATO????
void Privmsg::cmd_exec(int &fd)
{
	if (response != "") {
		if (response == "exit")
			return;
		if (this->type == "PRIVMSG")
			send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
		return;
	}
	std::vector<std::string> names;
	std::string text;
	for (int i = 2; command[i] != ""; i++)
			text += " " + command[i];
		text.erase(0, 1);
	for (std::map<std::string, std::string>::iterator it = cmd_map.begin(); it != cmd_map.end(); it++) {
		if (std::find(names.begin(), names.end(), it->first) != names.end())
			response = ERR_TOOMANYTARGETS(clients[fd]->Get_nickname());
		else
			names.push_back(it->first);
		if (it->first[0] != '#' && response == "") {
			if (cl_db.find(it->first) == cl_db.end())
				response = ERR_NOSUCHNICK(it->first);
			else
			{
				response = RPL_TEXT(clients[fd]->Get_nickname(), clients[fd]->Get_username(), it->first, type, text);
				send(cl_db[it->first], response.c_str(), response.length(), MSG_NOSIGNAL);
				response = "";
			}
		}
		else if (response == "")
		{
			if (channels.find(it->first) == channels.end())
				response = ERR_NOSUCHNICK(it->first);
			else if (clients[fd]->cl_channels.find(it->first) == clients[fd]->cl_channels.end())
				response = ERR_CANNOTSENDTOCHAN(it->first);
			else
				Command::send_all(*channels[it->first], fd, RPL_TEXT(clients[fd]->Get_nickname(), clients[fd]->Get_username(), it->first, type, text));
		}
		if (response != "" && type == "PRIVMSG")
			send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
		response = "";
    }
}