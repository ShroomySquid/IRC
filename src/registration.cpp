#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"

int check_pass(char* buffer, Client &client, std::string password) {
	cout << "Password is: " << password << endl;
	if (client.is_authentified()) {
		send(client.get_fd(), "You may not reregister\n", 24, 0);
		return (1);
	}
	if (!password.compare(buffer)) {
		client.authentify();
		cout << "Client " << client.get_fd() << " is authentified." << endl;
	}
	else {
		send(client.get_fd(), "Password incorrect\n", 20, 0);
		cout << "Client n." << client.get_fd() << " gave incorrect password.";
   		cout << buffer << endl;
	}
	return (0);
}

bool check_invalid_symbols(char *buffer) {
	int i = 0;
	while (buffer[i]) {
		if (i > 20)
			return (true);
		if (!i && (buffer[i] == '#' || buffer[i] == ':'))
			return (true);
		if (buffer[i] == 32 || (buffer[i] < 14 && buffer[i] > 8))
			return (true);
		i++;
	}
	return (false);
}

int set_nickname(char *buffer, Client &client, std::map<int, Client*> clients) {
	if (!client.is_authentified()) {	
		send(client.get_fd(), "Provide the password first\n", 28, 0);
		return (1);
	}
	if (client.get_nickname().length()) {	
		send(client.get_fd(), "Nickname already registered\n", 29, 0);
		return (1);
	}
	if (!buffer || !buffer[0]) {	
		send(client.get_fd(), "No nickname given\n", 19, 0);
		return (1);
	}
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (!it->second->get_nickname().compare(buffer)) {
			send(client.get_fd(), "Nickname is already in use\n", 27, 0);
			return (1);
		}
	}
	if (check_invalid_symbols(buffer)) {	
		send(client.get_fd(), "Erroneus nickname\n", 19, 0);
		return (1);
	}
	client.set_nickname(buffer);
	cout << "Client " << client.get_fd() << " set nickname to: ";
	cout << buffer << endl;
	return (0);
}


int set_username(char *buffer, Client &client, std::map<int, Client*> clients) {
	if (!client.is_authentified()) {	
		send(client.get_fd(), "Provide the password first\n", 28, 0);
		return (1);
	}
	if (client.get_username().length()) {	
		send(client.get_fd(), "Username already registered\n", 29, 0);
		return (1);
	}
	if (!buffer || !buffer[0]) {	
		send(client.get_fd(), "No username given\n", 19, 0);
		return (1);
	}
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (!it->second->get_username().compare(buffer)) {
			send(client.get_fd(), "Username is already in use\n", 28, 0);
			return (1);
		}
	}
	if (check_invalid_symbols(buffer)) {	
		send(client.get_fd(), "Erroneus username\n", 19, 0);
		return (1);
	}
	client.set_username(buffer);
	cout << "Client " << client.get_fd() << " set username to: ";
	cout << buffer << endl;
	client.regist();
	return (0);
}

int registration (Client &client, std::string password, char* buffer, std::map<int, Client*> clients) {	int i = 0;
	while (buffer[i]) {
		if (buffer[i] == '\n')
			buffer[i] = '\0';
		else
			i++;
	}
	if (!client.is_authentified())
		return (check_pass(buffer, client, password));
	if (!client.get_nickname().compare(""))
		return (set_nickname(buffer, client, clients));
	if (!client.get_username().compare(""))
		return (set_username(buffer, client, clients));
	//cout << client.get_fd() << " is in registration" << endl;
	return (0);
}
