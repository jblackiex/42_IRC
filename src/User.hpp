#pragma once


class User : public Command {
    public:
        User();
        
        void cmd_exec(int &fd);
        void cmd_gen_response(std::string &msg, int fd);
        ~User();
};