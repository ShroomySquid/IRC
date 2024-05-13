#include "../inc/IRC.hpp"

int test_server(int socketD, struct sockaddr_in *address) {
	bool is_online = true;
	int client_FD;
	char buffer[1024];
	struct sockaddr_in clientAddress;
	unsigned int clientAddressSize = sizeof(clientAddress);
	int bind_result = bind(socketD, (struct sockaddr *)address, sizeof(*address));
	if (bind_result != 0) {
		cout << "bind failed." << endl;
		return (1);
	}
	cout << "Waiting for client to connect..." << endl;
	if (listen(socketD, 10) == -1) {
		cout << "listen failed." << endl;
		return (1);
	}
	client_FD = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);
	if (client_FD < 0) {
		cout << "accept failed." << endl;
		close(client_FD);
		return (1);
	}
	cout << "Client message accepted." << endl;

	while (is_online) {
		if (recv(client_FD, buffer, 1024, 0) == -1) {
			cerr << "recv failed: " << std::strerror(errno) << endl;
			close(client_FD);
			return (1);
		}
		cout << "Client send: " << buffer << endl;
		bzero(buffer, 1024);
	}
	close(client_FD);
	return (0);
}

int main(int argc, char** argv) {
	/*
	if (argc != 3)
	{
		cerr << "Invalid amount of arguments provided. Please provide a port";
		cerr << " and a server password." << endl;
		return (1);
	}
	*/
	(void)argc;
	(void)argv;
	int socketD = create_socket_descriptor();
	if (socketD == -1)
		return (1);
	char address_str[] = "";
	struct sockaddr_in* address = set_address(address_str, 2000);
	if (address == NULL)
		return (1);
	test_server(socketD, address);
	free(address);
	close(socketD);
	return (0);
}
