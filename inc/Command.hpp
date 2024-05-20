#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "IRC.hpp"
# include <vector>

#include <string>
#include <vector>
#include <iostream>
#include "Client.hpp"
// Commande interface
class Command
{
    private:
        /* data */
    public:
        Command(){};
        virtual ~Command(){};
        virtual void execute(Client& sender, std::vector<std::string> arguments) = 0;
};

class Cmd_join : public Command
{
    public:
        Cmd_join();
        ~Cmd_join();
        void execute(Client& sender, std::vector<std::string> arguments);
};

class Cmd_kick : public Command
{
    public:
        Cmd_kick();
        ~Cmd_kick();
        void execute(Client& sender, std::vector<std::string> arguments);
};

class Cmd_privmsg : public Command
{
    public:
        Cmd_privmsg();
        ~Cmd_privmsg();
        void execute(Client& sender, std::vector<std::string> arguments);
};

#endif
