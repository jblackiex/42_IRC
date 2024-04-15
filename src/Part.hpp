#pragma once

class Part : public Command {
    public:
        Part();

        void cmd_exec(int &fd);
        void cmd_gen_response(std::string &msg, int fd);
        ~Part();
};