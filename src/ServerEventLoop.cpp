#include "../inc/Server.hpp"
#include "../inc/Command.hpp"

void Server::Run() {
    DEBUG_PRINT("Running server...");
    ListenClients();
    while (true) {
        PollAndProcessClients();
        MarkAndRemoveDisconnectedClients();
    }
}

void Server::ListenClients() {
    DEBUG_PRINT("Waiting for clients to connect...");
    if (listen(socketD, 10) == -1) {
        cout << "listen failed." << endl;
        throw std::exception();
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
    int infd = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (infd > 0)
        login_attempt(clients, infd);
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

void Server::ProcessClientMessage(const pollfd& pfd) {
    int bytesReceived = recv(pfd.fd, recv_buffer, 1024, 0);
	//cout << "recv_buffer: " << recv_buffer << endl;
    if (bytesReceived <= 0) {
        std::map<int, Client*>::iterator it = clients.find(pfd.fd);
        if (it != clients.end()) {
            it->second->disconnect();
            need_to_remove_client = true;
        }
        return;
    }
    if (recv_buffer[0] != '\0') {
		if (!append_buffer()) {
			bzero(buffer, 1024);
			bzero(recv_buffer, 1024);
			buffer_len = 0;
			return ;
		}
        //cout << "buffer (after append): " << buffer << endl;
        bzero(recv_buffer, 1024);
        if (!is_IRC_message(buffer))
            return;
        buffer[buffer_len - 2] = '\0';
		std::map<int, Client*>::iterator it = clients.find(pfd.fd);
        if (it != clients.end()) {
            cout << "buffer: " << buffer << endl;
            process_message(*this, *(it->second), commands, buffer);
        }
        bzero(buffer, 1024);
		buffer_len = 0;
    }
}

void Server::process_message(Server &server, Client &sender, std::map<std::string, Command*>& commands, char *input)
{
	process_called++;
	cout << process_called << endl;
	std::vector<std::string> args; // arguments de la commande.
    char *token = std::strtok(input, " ");
	std::string cmd;
	if (!token)
		cmd = "";
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
