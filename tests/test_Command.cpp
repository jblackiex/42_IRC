#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../src/ft_irc.h"
#include "doctest.h"

class CommandConcrete : public Command {
    public:
        CommandConcrete(std::string type) : Command(type) {}
        void cmd_gen_response(std::string &msg, int fd) {}
        void cmd_exec(int &fd) {}
};


TEST_CASE("Command parsing") {
    CommandConcrete cmd("JOIN #channel");
    // CHECK(2 + 2 == 4);
    CHECK(cmd.Get_type() == "JOIN #channel");
    
    // CHECK(cmd.getParams() == "#channel");
}

// TEST_CASE("Command errors") {
//     // CHECK_THROWS_AS(Command(""), std::invalid_argument);
// }