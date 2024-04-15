#include "ft_irc.h"
Channel::Channel(const std::string &ch_name) {
    this->ch_name = ch_name;
    ch_topic = "GPT-3.5 turbo Chat_Bot --> type CHATGPT to start a conversation with GPT \
    --> type CHATGPT <message> to ask GPT a question \
    --> type CHATGPT ROLE <role> to set the role of the conversation \
    --> type CHATGPT OFF to stop the conversation";
    ch_pass = "";
    user_ch_limit = 10;
    ch_users = 0;
    invite_only = 0;
    op_topic = 1;
}

std::string &Channel::Get_chname() {
    return ch_name;
}

std::string &Channel::Get_chtopic() {
    return ch_topic;
}

std::string &Channel::Get_chpass() {
    return ch_pass;
}

int &Channel::Get_userchlimit() {
    return user_ch_limit;
}

int &Channel::Get_chusers() {
    return ch_users;
}

int &Channel::Get_inviteonly() {
    return invite_only;
}

int &Channel::Get_optopic() {
    return op_topic;
}

void Channel::Set_chpass(const std::string &ch_pass) {
    this->ch_pass = ch_pass;
}

void Channel::Set_chtopic(const std::string &ch_topic) {
    this->ch_topic = ch_topic;
}


void Channel::Set_userchlimit(int user_ch_limit) {
    this->user_ch_limit = user_ch_limit;
}





Channel::~Channel() {
}

