#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"
#include "../inc/Command.hpp"

// void broadcastAll(std::map<std::string, Client*>& clients, std::string except, char *buffer)
// {
// 	for (std::map<std::string, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
// 	{
// 		if (it->first != except)
// 			send(it->second->get_fd(), buffer, 1024, 0);
// 	}
// }

void process_message(Client &sender,std::map<std::string, Command*>& commands, std::string input)
{
	std::string cmd; // nom de la commande.
	std::vector<std::string> args; // arguments de la commande.
    char *token = std::strtok((char *)input.c_str(), " ");
	cmd = token;
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL)
    {
        args.push_back(token);
        token = std::strtok(NULL, " ");
    }
	if (commands.find(cmd) != commands.end())
		commands[cmd]->execute(sender, args); // execute la commande
	else
		std::cout << "This command doesnt exist !" << std::endl;
}
