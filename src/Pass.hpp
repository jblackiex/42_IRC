#pragma once

#define ERR_PASSWDMISMATCH(wrongpass) ":ft_irc.network.eu 462 " + wrongpass + " :Wrong password\r\n"

class Pass : public Command {
    public:
        Pass();

        void cmd_exec(int &fd);
        void cmd_gen_response(std::string &msg, int fd);
        ~Pass();
};