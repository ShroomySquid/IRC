#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include "../inc/ResponseHandler.hpp"

void Server::Run() {
    DEBUG_PRINT("Running server...");
    ListenClients();
    while (online) {
        PollAndProcessClients();
        MarkAndRemoveDisconnectedClients();
        initSignals();
    }
    close(socketD);
    // Full cleanup if signal is received
}

void Server::ListenClients() {
    DEBUG_PRINT("Waiting for clients to connect...");
    if (listen(socketD, 10) == -1) {
        perror("listen");
        return;
    }
}

void Server::PollAndProcessClients() {
    std::vector<pollfd> pfds;
    CreatePollfds(pfds);
    int poll_count = poll(&pfds[0], pfds.size(), 0); // 0 seconds means poll will return immediately without blocking.

    if (poll_count < 0) {
        perror("poll");
        return;
    }

    for (std::vector<pollfd>::iterator pfd_it = pfds.begin(); pfd_it != pfds.end(); ++pfd_it) {
        if (pfd_it->fd == socketD && pfd_it->revents & POLLIN) {
            AcceptClients();
        } else if (pfd_it->revents & POLLIN) {
            ProcessClientMessage(*pfd_it);
        }
    }
}

void Server::CreatePollfds(std::vector<pollfd>& pfds) {
    pollfd listeningSocket;
    listeningSocket.fd = socketD;
    listeningSocket.events = POLLIN;
    listeningSocket.revents = 0;
    pfds.push_back(listeningSocket);

    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (!it->second->is_disconnected()) {
            pollfd pfd;
            pfd.fd = it->second->get_fd();
            pfd.events = POLLIN | POLLOUT;
            pfd.revents = 0;
            pfds.push_back(pfd);
        }
    }
}

void Server::AcceptClients() {
    DEBUG_PRINT("Accepting clients...");
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int infd = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);

    if (infd > 0) {
        // 1. Create a new client object
        Client* newClient = new Client(infd);
        if (newClient == NULL) {
            perror("new");
            return;
        }

        // 2. Register the client
        registerClient(clients, newClient);

        // 3. Extract IP and Port
        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIp, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddress.sin_port);

        // 5. Log the connection
        sendServerMsg("Connection established from %s:%d", clientIp, clientPort);
    }
}

bool Server::append_buffer(void) {
	int a = 0;
	while (buffer[buffer_len])
		buffer_len++;
	while (recv_buffer[a] && (buffer_len + a) < 1024) {
		buffer[buffer_len + a] = recv_buffer[a];
		a++;
	}
	buffer_len += a;
	if (buffer_len <= 2)
		return (true);
	if ((buffer_len) >= 1024 && (buffer[buffer_len - 2] != '\r' || buffer[buffer_len - 1] != '\n')) {
		cout << "Invalid message: message too long without being terminated" << endl;
		return (false);
	}
	return (true);
}

void Server::Get_rid_of_newlines(char *buffer) {
	// version qui enlève les /n
	/*
	int i = 0;
	int a = 0;
	while (buffer[i] && buffer[i + a] && buffer[i + 1]) {
		if (buffer[i + a] == '\n' && (i == 0 || buffer[i + a - 1] != '\r')) {
			while (buffer[i + a] && buffer[i + a] == '\n')
				a++;
		}
		if (a && buffer[i + a])
			buffer[i] = buffer[i + a];
		i++;
	}
	if (buffer[i + 1])
		buffer[i + 1] = '\0';
	*/
	// version qui remplace les /n par des espaces
	int i = 0;
	while (buffer[i]) {
		if (buffer[i] == '\n' && (i == 0 || buffer[i - 1] != '\r'))
			buffer[i] = ' ';
		i++;
	}
}

void Server::Split_message(Client* client, char *buffer) {
	int i = 0;
	while (buffer[i] && buffer[i + 1] && !(buffer[i + 1] == '\n' && buffer[i] == '\r'))
		i++;
	buffer[i] = '\0';
	process_message(*this, *client, commands, buffer);
	if (buffer[i + 2])
		Split_message(client, &buffer[i + 2]);
}

void Server::ProcessClientMessage(const pollfd& pfd) {
    int bytesReceived = recv(pfd.fd, recv_buffer, 1024, 0);    
    if (bytesReceived <= 0) {
        std::map<int, Client*>::iterator it = clients.find(pfd.fd);
        if (it != clients.end()) {
            it->second->disconnect();
            need_to_remove_client = true;
        }
        return;
    }
	if (bytesReceived < 2 && recv_buffer[0] == '\n')
		recv_buffer[0] = '\0';

    // [debug] prints all received bytes from client on fd
    DEBUG_PRINT_HEX(recv_buffer, bytesReceived);
    DEBUG_PRINT("Received " << bytesReceived << " bytes from client on fd " << pfd.fd << ": " << recv_buffer);


    if (recv_buffer[0] != '\0') {
		if (!append_buffer()) {
			bzero(buffer, buffer_len);
			bzero(recv_buffer, bytesReceived);
			buffer_len = 0;
			return ;
		}
        bzero(recv_buffer, bytesReceived);
        if (!is_IRC_message(buffer))
            return;
		std::map<int, Client*>::iterator it = clients.find(pfd.fd);
        if (it != clients.end()) {
            cout << "buffer: " << buffer;
			Get_rid_of_newlines(buffer);
            //cout << "buffer after get_rid: " << buffer;
			Split_message(it->second, buffer);
        }
        bzero(buffer, buffer_len);
		buffer_len = 0;
    }
}


void Server::process_message(Server &server, Client &sender, std::map<std::string, Command*>& commands, char *input)
{
	std::vector<std::string> args; // arguments de la commande.
    char *token = std::strtok(input, " ");
	std::string cmd;
	if (!token || token[0] == '\n')
		return ;
	else
		cmd = token;
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL)
    {
        args.push_back(token);
        token = std::strtok(NULL, " ");
    }
	if (commands.find(cmd) != commands.end())
		commands[cmd]->execute(server, sender, args); // execute la commande
	else
	 	std::cout << cmd << " : Command not found" << std::endl;
}

void Server::MarkAndRemoveDisconnectedClients() {
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ) {
        if (it->second->is_disconnected()) {
            DEBUG_PRINT("Closing client on fd " << it->second->get_fd());
            close(it->second->get_fd());
            delete it->second;
            std::map<int, Client*>::iterator to_erase = it++;
            clients.erase(to_erase);
        } else {
            ++it;
        }
    }
}
