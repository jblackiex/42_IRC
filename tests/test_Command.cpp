#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../src/ft_irc.h"
#include "doctest.h"

class CommandConcrete : public Command {
    public:
        CommandConcrete(std::string type) : Command(type) {}
        void cmd_gen_response(std::string &msg, int fd) {}
        void cmd_exec(int &fd) {}
};
CommandConcrete cmd("JOIN #channel");

TEST_CASE("Command constructor") {
    CHECK(cmd.Get_type() == "JOIN #channel");
}

TEST_CASE("Command space_checker") {
    std::string msg_with_space = "Hello World";
    std::string msg_without_space = "HelloWorld";
    CHECK(cmd.space_checker(msg_with_space) == false);
    CHECK(cmd.space_checker(msg_without_space) == false);
}

TEST_CASE("Command Set_commands and Get_commands") {
    std::string commands[17] = {"cmd1", "cmd2", "cmd3", "cmd4", "cmd5", "cmd6", "cmd7", "cmd8", "cmd9", "cmd10", "cmd11", "cmd12", "cmd13", "cmd14", "cmd15", "cmd16", "cmd17"};
    int INIT_POS = 0;
    cmd.Set_commands(commands);
    CHECK(cmd.Get_commands(INIT_POS).length() == 92);
}