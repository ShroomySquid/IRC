#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"

void login_attempt(std::map<int, Client*> &clients, int infd) {
	Client* received_client = new Client(infd);
	// error if new fail
	if (received_client != NULL) {
		clients.insert(std::pair<int, Client*> 
				(received_client->get_fd(), received_client));
	}
}

int check_pass(char* buffer, Client &client, std::string password) {
	cout << "Password is: " << password << endl;
	if (!password.compare(buffer)) {
		client.authentify();
		cout << "Client " << client.get_fd() << " is authentified." << endl;
	}
	else {
		cout << "Client " << client.get_fd() << " gave wrong password: ";
   		cout << buffer << endl;
	}
	return (0);
}

int set_nickname(char *buffer, Client &client, std::map<int, Client*> clients) {
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (!it->second->get_nickname().compare(buffer)) {
			cout << "Nickname " << buffer << " already taken." << endl;
			return (1);
		}
	}
	cout << "Client " << client.get_fd() << " set nickname to: ";
	cout << buffer << endl;
	client.set_nickname(buffer);
	return (0);
}


int set_username(char *buffer, Client &client, std::map<int, Client*> clients) {
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (!it->second->get_username().compare(buffer)) {
			cout << "Nickname " << buffer << " already taken." << endl;
			return (1);
		}
	}
	cout << "Client " << client.get_fd() << " set username to: ";
	cout << buffer << endl;
	client.set_username(buffer);
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
