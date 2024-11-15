#include "../src/ft_irc.h"
#include "doctest.h"

Invite invite;

TEST_CASE("Invite command generation response") {
    std::string msg = "INVITE user #channel";
    int fd = 1;
    invite.cmd_gen_response(msg, fd);
    CHECK(invite.Get_type() == "INVITE");
}

TEST_CASE("Invite command execution") {
    int fd = 1;
    invite.cmd_exec(fd);
    CHECK(invite.Get_type() == "INVITE");
}