#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"

Client* new_client(int fd, std::string username, std::string nickname) {
	Client* new_client = new Client(fd, username, nickname);
	return (new_client);
}

void login_attempt(std::map<int, Client*> &clients, int infd, std::string password) {
	(void)password;
	Client* received_client = new_client(infd, "user", "nick");
	if (received_client != NULL) {
		clients.insert(std::pair<int, Client*> 
				(received_client->get_fd(), received_client));
		cout << "Client " << received_client->get_username();
		cout << " connected." << endl;
	}
}
