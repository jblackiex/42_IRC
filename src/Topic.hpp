#pragma once

class Topic : public Command
{
	public:
		Topic();
		~Topic();

		void cmd_gen_response(std::string &msg, int fd);
		void cmd_exec(int &fd);
};