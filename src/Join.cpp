#include "ft_irc.h"
#include "Join.hpp"

Join::Join() : Command("JOIN") {}

Join::~Join() {}

void Join::cmd_gen_response(std::string &msg, int fd)
{
	(void) msg;
	response = "";
	Command::check_prefix(fd);
	if (response != "") return;
	response = command[0] != "JOIN" ? ERR_UNKNOWNCOMMAND(Get_type()) : response;
	response = response == "" && command[1].empty() ? ERR_NEEDMOREPARAMS(Get_type()) : response;
	parse_command();
}

// SE NON CI SONO PIU UTENTI NEL CANALE IL CANALE VIENE ELIMINATO????
void Join::cmd_exec(int &fd)
{
	int added_now = 0;
	if (response != "") {
		send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
		return;
	}

	Client *client = clients[fd];
	for (std::map<std::string, std::string>::iterator it = cmd_map.begin(); it != cmd_map.end(); it++) {
		if (it->first[0] != '#') {
			response = ERR_NOSUCHCHANNEL(it->first);
		}
        else if (channels.find(it->first) == channels.end()) {
			channels.insert(std::pair<std::string, Channel *>(it->first, new Channel(it->first)));
			client->cl_channels.insert(std::pair<std::string, int>(it->first, 1));
			if (it->second != "")
				channels[it->first]->Set_chpass(it->second);
			added_now = 1;
        }//client gia nel canale	
		else if (channels[it->first]->Get_chpass() != "" && channels[it->first]->Get_chpass() != it->second) {
			response = ERR_BADCHANNELKEY(it->first);
		}
		else if (channels[it->first]->Get_chusers() >= channels[it->first]->Get_userchlimit()) {
			response = ERR_CHANNELISFULL(it->first);
		}
		else if (channels[it->first]->Get_inviteonly()) {
			if (std::find(clients[fd]->invites.begin(), clients[fd]->invites.end(), it->first) == clients[fd]->invites.end())
				response = ERR_INVITEONLYCHAN(it->first);
			else
				clients[fd]->invites.erase(std::remove(clients[fd]->invites.begin(), clients[fd]->invites.end(), it->first), clients[fd]->invites.end());
		}
		if (response == "") {		// else if (channels[it->first]->Get_inviteonly() == 1) {
			if (!added_now && client->cl_channels.find(it->first) != client->cl_channels.end())
				continue;
			++channels[it->first]->Get_chusers();
			client->cl_channels.insert(std::pair<std::string, int>(it->first, 0));
			response = ":" + client->Get_nickname() + "!" + client->Get_username() + "@" + "ft_irc.network.eu" + " JOIN :" + it->first + "\r\n";
			Command::send_all(*channels[it->first], -1, response);
			if (channels[it->first]->Get_chtopic() != "")
			{
				response = RPL_TOPIC(it->first, channels[it->first]->Get_chtopic(), client->Get_nickname());
				send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
			}
			response = RPL_NAMREPLY(it->first, client->Get_nickname(), Get_nicknames(*channels[it->first]));
			send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
			response = RPL_ENDOFNAMES(it->first, client->Get_nickname());
			send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
		}
		else {
			send(fd, response.c_str(), response.length(), MSG_NOSIGNAL);
		}
		response = "";
		added_now = 0;
    }
}