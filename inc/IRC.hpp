#ifndef IRC_HPP
# define IRC_HPP
# define _XOPEN_SOURCE_EXTENDED 1
# include <string>
# include <string.h>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <map>
# include <poll.h>
# include <fcntl.h>
# include <cstdlib>
# include <vector>

using std::cout;
using std::cerr;
using std::endl;

class IRC {
	public:
		IRC(void);
		IRC(const IRC& old);
		~IRC(void);
		IRC & operator=(IRC const & rhs);
};

int		create_socket_descriptor(void);
struct	sockaddr_in *set_address(char *ip, int port);

#endif
