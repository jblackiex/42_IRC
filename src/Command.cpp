#include "ft_irc.h"
Command::Command(std::string type) : type(type) {}

Command *Command::cmd_select(std::string &msg, int fd) {
	Command *cmd = NULL;
	if (msg.find("PING") != std::string::npos) {
		std::string send_client = "PONG " + msg.substr(msg.find("PING") + 4) + "\r\n";
		send(fd, send_client.c_str(), send_client.size(), MSG_NOSIGNAL);
		return NULL;
	}
	int flag = clients[fd]->Get_flag();
	if (!flag && msg.find("PASS") != std::string::npos) {
		cmd = server.cmds["PASS"];
	} else if (flag == 1 || flag == 2) {
		cmd = clients[fd]->Get_username() == "" && (flag == 1 || flag == 2) && (msg.find("USER") != std::string::npos) ? server.cmds["USER"] : NULL;
		if (!cmd && clients[fd]->Get_nickname() == "")
			cmd = (flag == 1 || flag == 2) && (msg.find("NICK") != std::string::npos) ? server.cmds["NICK"] : NULL;
	}
	else if (flag > 2) {
		cmd = server.Get_cmd(msg);
		if (cmd)
			cmd = cmd->Get_type() == "CHATGPT" && msg.find("PRIVMSG") != std::string::npos ? server.cmds["PRIVMSG"] : cmd; 
		if (cmd && cmd->Get_type() == "CHATGPT" && clients[fd]->Get_gptfd() < 0 && msg.length() < 10) {
			msg = "gpt";
		}
		if (cmd && cmd->Get_type() != "CHATGPT" && msg.length() > 512) {
            msg = msg.substr(0, 510);
            msg += "\r\n";
        }
	}
	cmd && cmd->space_checker(msg) ? 0 : cmd = NULL;
	if (cmd)
		cmd->cmd_gen_response(msg, fd); // ogni Command parsa in modo diverso
	return cmd;
}

void Command::Set_commands(std::string cmdz[17]) {
        for (int i = 0; i < 17; i++) {
            command[i] = cmdz[i];
            if (command[i] == "")
                break;
        }
}
bool Command::space_checker(std::string &msg) {
	std::fill(command, command + 15, std::string(""));
	int flag = 0;
	if (msg == "gpt") return true;
	else if (msg.find("CHATGPT") != std::string::npos) flag = 1;
	 if (msg.empty() || msg[0] == ' ') {
            return false; // Controlla spazi all'inizio o alla fine
        }

		int i = 0;
        std::istringstream stream(msg);
        std::string word;
        std::string rebuiltMsg;

		size_t pos1 = msg.find("\n");
		size_t pos2 = msg.find("\r\n");

		size_t j;
		if (pos1 != std::string::npos) {
        // Trova l'inizio della sequenza di spazi che precede \r\n
			j = pos1;
			while (j > 0 && msg[j-1] == ' ') {
				--j;
				msg.erase(j, 1);
			}
			// Rimuove gli spazi
		} else {
			j = pos2;
			while (j > 0 && msg[j-1] == ' ') {
				--j;
				msg.erase(j, 1);
			}
		}
		// Ricostruisce la stringa parola per parola
        while (stream >> word) {
			if (i > 16)
				return false;
			command[i] = word;
			++i;
            if (!rebuiltMsg.empty()) {
                rebuiltMsg += " "; // Aggiunge uno spazio tra le parole
            }
            rebuiltMsg += word;
			if (flag && i > 2)
				break;
        }
		if (flag)
			return true;
		if (pos2 != std::string::npos)
			rebuiltMsg += "\r\n";
		else
			rebuiltMsg += "\n";
        // Confronta la stringa ricostruita con quella originale
        return msg == rebuiltMsg;
} 

void Command::send_all(Channel channel, int fd, std::string msg) {

	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (it->first != fd && it->second->cl_channels.find(channel.Get_chname()) != it->second->cl_channels.end())
			send(it->second->Get_fd(), msg.c_str(), msg.size(), MSG_NOSIGNAL);
	}
}

void Command::check_prefix(int fd) {
	if (command[0].find(':') == 0) {
		command[0].erase(0, 1);
		if (command[0] != clients[fd]->Get_nickname())
			response = ERR_INVALIDPREFIX(command[0]);
		else {
			for (int i = 0; i < 16; ++i) {
    			command[i] = command[i + 1]; 
				if (command[i] == "")
					break;
			}
		}
	}
}

void Command::parse_command() {
	std::istringstream stream1(command[1]);
	std::istringstream stream2(command[2]);
	std::string cmd;
	std::string pass;

	cmd_map.clear();
	while (std::getline(stream1, cmd, ',')) {
		if (command[0] == "JOIN" && command[2] != "" && std::getline(stream2, pass, ','))
			cmd_map.insert(std::pair<std::string, std::string>(cmd, pass));
		else
			cmd_map.insert(std::pair<std::string, std::string>(cmd, ""));
	}
}


std::string Command::Get_commands(int i) {
	std::string commands = "";
	while (command[i] != "") {
		commands += command[i++];
		if (command[i] != "")
			commands += " ";
	}
	return commands;
}

std::string &Command::Get_type() {
    return type;
}

Command::~Command() {
	bzero(command, 15);
	response.clear();
	type.clear();
}

std::string Command::Get_nicknames(Channel channel)
{
	std::string nicknames = "";
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (it->second->cl_channels.find(channel.Get_chname()) != it->second->cl_channels.end()) {
			if (it->second->cl_channels[channel.Get_chname()])
				nicknames += " @" + it->second->Get_nickname();
			else
				nicknames += " " + it->second->Get_nickname();
		}	
	}
	nicknames.erase(0, 1);
	return nicknames;
}




