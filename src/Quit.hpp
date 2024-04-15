#pragma once

class Quit : public Command {
    public:
        Quit();

        void cmd_exec(int &fd);
        void cmd_gen_response(std::string &msg, int fd);
        ~Quit();
};