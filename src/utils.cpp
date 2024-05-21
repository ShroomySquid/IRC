#include "../inc/IRC.hpp"

int create_socket_descriptor(void) {
	int socketD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketD < 0) {
		cout << "Socket failed" << endl;
		return (-1);
	}
	int optval = 1;
	setsockopt(socketD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval) < 0);
	return (socketD);
}

struct sockaddr_in *set_address(char *ip, int port) {
	struct sockaddr_in *address = (struct sockaddr_in*)malloc(sizeof(sockaddr_in));
	if (address == NULL) {
		cout << "malloc failed." << endl;
		return (NULL);
	}
	address->sin_port = htons(port);
	address->sin_family = AF_INET;
	if (strlen(ip) == 0)
		address->sin_addr.s_addr = INADDR_ANY;
	else
		inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
	return (address);
}

bool check_invalid_symbols(std::string buffer) {
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

void not_registered_yet(int fd) {
	send(fd, "Not registered yet. Cannot execute command.", 44, 0);
}
