#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "IRC.hpp"
# include <vector>

#include <string>
#include <vector>
#include <iostream>
#include "Client.hpp"
class Server;
// Commande interface
class Command
{
    private:
        /* data */
    public:
        Command(){};
        virtual ~Command(){};
        virtual void execute(Server &server, Client& sender, std::vector<std::string> arguments) = 0;
};

class Cmd_join : public Command
{
    public:
        Cmd_join();
        ~Cmd_join();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
		void fill_chan_to_join(std::vector<std::string> &chan_to_join, std::string channels);
		int	check_errors(int *pass_start, Channel* channel, std::string passwords, Client& sender);
		int init(Client& sender, std::vector<std::string> arguments);
		std::string return_pass(int *pass_start, std::string passwords);
};

class Cmd_kick : public Command
{
    public:
        Cmd_kick();
        ~Cmd_kick();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_privmsg : public Command
{
    public:
        Cmd_privmsg();
        ~Cmd_privmsg();
		void parse_targets(std::vector<std::string> &target_vec, std::string targets);
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_nick : public Command
{
    public:
        Cmd_nick();
        ~Cmd_nick();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_user : public Command
{
    public:
        Cmd_user();
        ~Cmd_user();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_pass : public Command
{
    public:
        Cmd_pass();
        ~Cmd_pass();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_cap : public Command
{
    public:
        Cmd_cap();
        ~Cmd_cap();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};
class Cmd_topic : public Command
{
    public:
        Cmd_topic();
        ~Cmd_topic();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_mode : public Command
{
    public:
        Cmd_mode();
        ~Cmd_mode();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_part : public Command
{
    public:
        Cmd_part();
        ~Cmd_part();
		void fill_chan_to_quit(std::vector<std::string> &chan_to_quit, std::string channels);
		void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_invite : public Command
{
    public:
        Cmd_invite();
        ~Cmd_invite();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_quit : public Command
{
    public:
        Cmd_quit();
        ~Cmd_quit();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

class Cmd_ping : public Command
{
    public:
        Cmd_ping();
        ~Cmd_ping();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

#endif
