#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "IRC.hpp"
# include <vector>

// Commande interface
class Command
{
    private:
        /* data */
    public:
        Command(){};
        virtual ~Command(){};
        virtual void execute(std::vector<std::string> arguments) = 0;
};

class Cmd_join : public Command
{
    public:
        Cmd_join();
        ~Cmd_join();
        virtual void execute(std::vector<std::string> arguments);
};

class Cmd_kick : public Command
{
    public:
        Cmd_kick();
        ~Cmd_kick();
        virtual void execute(std::vector<std::string> arguments);
};

class Cmd_privmsg : public Command
{
    public:
        Cmd_privmsg();
        ~Cmd_privmsg();
        virtual void execute(std::vector<std::string> arguments);
};

#endif
