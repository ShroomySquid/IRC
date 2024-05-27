#include "../inc/IRC.hpp"
#include "../inc/Client.hpp"
#include "../inc/Command.hpp"
#include "../inc/Server.hpp"

int check_port(std::string port) {
	int i = 0;
	while (port[i]) {
		if (!isdigit(port[i])) {
			cout << "Invalid character in port. Please only put digits." << endl;
			return (-1);
		}
		i++;
	}
	if (i > 5) {
		cout << "Invalid port : port too big" << endl;
		return (-1);
	}
	try {
		int port_int = std::atoi(port.c_str());
		if (port_int > 65535 || port_int < 1025) {
			cout << "Invalid port." << endl;
			return (-1);
		}
		return (port_int);
	}
	catch (...) {
		cout << "error with stoi." << endl;
		return (-1);
	}
	return (-1);
}

bool invalid_password(std::string password) {
	if (password.length() < 5 || password.length() > 30) {
		cout << "Invalid password size. Please provide a password with more";
		cout << " than 5 character and less than 30." << endl;
		return (true);
	}
	return (false);
}

int main(int argc, char** argv)
{
	
	if (argc != 3)
	{
		cerr << "Invalid amount of arguments provided.";
		cerr << " ./ircserv <port> <password>" << endl;
	 	return (1);
	}
	int port = check_port(argv[1]);
	if (port < 0 || invalid_password(argv[2])) {
		return (1);
	}
	int socketD = create_socket_descriptor();
	if (socketD == -1)
		return (1);
	char address_str[] = "";
	struct sockaddr_in* address = set_address(address_str, port);
	if (address == NULL)
		return (1);
	try
	{
		Server server(socketD, address, argv[2]);
		server.Run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	free(address);
	close(socketD);
	return (0);
}
