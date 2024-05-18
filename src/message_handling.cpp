#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"
#include "../inc/Command.hpp"

void broadcastAll(std::map<int, Client*>& clients, int except, char *buffer)
{
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->first != except && it->second->is_registered())
			send(it->second->get_fd(), buffer, 1024, 0);
	}
}

void process_message(std::map<std::string, Command*>& commands, char *buffer)
{
	std::string cmd; // nom de la commande.
	std::vector<std::string> args; // arguments de la commande.
	//cout << "Client send: " << buffer << std::endl;
	int i = 0;
	while (buffer[i] && buffer[i] != ' ' && buffer[i] != '\n')
	{
		cmd += buffer[i];
		i++;
	}
	if (buffer[i] == ' ')
		i++;
	while(buffer[i] && buffer[i] != '\n')
	{
		std::string arg;
		while (buffer[i] && buffer[i] != ' ')
		{
			arg += buffer[i];
			i++;
		}
		i++;
		args.push_back(arg);
	}

	if (commands.find(cmd) != commands.end())
		commands[cmd]->execute(args); // execute la commande
	else
		std::cout << "This command doesnt exist !" << std::endl;
}
