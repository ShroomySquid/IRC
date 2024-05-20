#ifndef IRC_HPP
# define IRC_HPP
# define _XOPEN_SOURCE_EXTENDED 1
# include <string>
# include <string.h>
# include <cstring>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <map>
# include <poll.h>
# include <fcntl.h>
# include <cstdlib>
# include "Channel.hpp"
# include <vector>

using std::cout;
using std::cerr;
using std::endl;

class Client;
class Command;

class IRC {
	public:
		IRC(void);
		IRC(const IRC& old);
		~IRC(void);
		IRC & operator=(IRC const & rhs);
};

int		create_socket_descriptor(void);
struct	sockaddr_in *set_address(char *ip, int port);
Client*	new_client(int fd, std::string username, std::string nickname);
void	login_attempt(std::map<std::string, Client*> &clients, int infd, std::string password);
//void broadcastAll(std::map<std::string, Client*>& clients, std::string except, char *buffer);
void process_message(Client &sender,std::map<std::string, Command*>& commands, char *buffer);

#endif
