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

class Cmd_topic : public Command
{
    public:
        Cmd_topic();
        ~Cmd_topic();
        void execute(Server &server, Client& sender, std::vector<std::string> arguments);
};

#endif
