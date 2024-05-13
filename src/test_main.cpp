#include "../inc/IRC.hpp"

int test_client(int socketD, struct sockaddr_in address) {
	bool server_online = true;
	int connect_test = connect(socketD, (struct sockaddr *)&address, sizeof(address));
	if (connect_test != 0) {
		cout << "connection failed." << endl;
		return (1);
	}
	cout << "connection was successful." << endl;

	std::string msg;
	//char buffer[1024];
	while (server_online && !std::cin.fail()) {
		cout << "> ";
		std::getline(std::cin, msg);
		if (send(socketD, msg.c_str(), msg.length(), 0) == -1) {
			cout << "Send failed." << endl;
			return (1);
		}
		cout << "send succeed." << endl;
		/*
		if (recv(socketD, buffer, 1024, 0) == -1) {
			cout << "recv failed." << endl;
			return (1);
		}
		cout << buffer << endl;
		*/
	}
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
	char address_str[] = "127.0.0.1";
	struct sockaddr_in* address = set_address(address_str, 2000);
	if (address == NULL)
		return (1);
	test_client(socketD, *address);
	close (socketD);
	free(address);
	return (0);
}
