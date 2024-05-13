#include "../inc/IRC.hpp"

int create_socket_descriptor(void) {
	int socketD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketD < 0) {
		cout << "Socket failed" << endl;
		return (-1);
	}
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

