#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"

/*
Client* new_client(int fd, std::string username, std::string nickname) {
	Client* new_client = new Client(fd, username, nickname);
	return (new_client);
}
*/

void login_attempt(std::map<int, Client*> &clients, int infd) {
	Client* received_client = new Client(infd);
	// error if new fail
	if (received_client != NULL) {
		clients.insert(std::pair<int, Client*> 
				(received_client->get_fd(), received_client));
	}
}

int check_pass(char* buffer, Client &client, std::string password) {
	int i = 0;
	while (buffer[i]) {
		if (buffer[i] == '\n')
			buffer[i] = '\0';
		//cout << "Buffer: " << buffer[i] << endl;
		else
			i++;
	}
	//cout << "i: " << i << endl;
	std::string buff = buffer;
	//buff.pop_back();
	cout << "Password is: " << password << endl;
	if (!password.compare(buff)) {
		client.authentify();
		cout << "Client " << client.get_fd() << " is authentified." << endl;
	}
	else {
		cout << "Client " << client.get_fd() << " gave wrong password: ";
   		cout << buffer << endl;
		cout << password.compare(buff) << endl;
	}
	return (0);
}

int registration (Client &client, std::string password, char* buffer, std::map<int, Client*> clients) {
	//(void)password;
	(void)clients;
	(void)buffer;
	if (!client.is_authentified())
		return (check_pass(buffer, client, password));
	//cout << client.get_fd() << " is in registration" << endl;
	return (0);
}
