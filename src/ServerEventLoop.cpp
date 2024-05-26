#include "../inc/Server.hpp"
#include "../inc/Command.hpp"
#include "../inc/ResponseHandler.hpp"

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
    sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int infd = accept(socketD, (struct sockaddr*)&clientAddress, &clientAddressSize);

    if (infd > 0) {
        login_attempt(clients, infd);

        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIp, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddress.sin_port);

        sendServerMsg("Connection established from %s:%d", clientIp, clientPort);
    }
}

void Server::ProcessClientMessage(const pollfd& pfd) {
    int bytesReceived = recv(pfd.fd, buffer, 1024, 0);
    if (bytesReceived <= 0) {
        std::map<int, Client*>::iterator it = clients.find(pfd.fd);
        if (it != clients.end()) {
            it->second->disconnect();
            need_to_remove_client = true;
        }
        return;
    }

    if (buffer[0] != '\0') {
        std::string receivedData(buffer, bytesReceived);
        if (!is_IRC_message(receivedData)) {
            cout << "Client sent invalid data: " << receivedData << endl;
            bzero(buffer, 1024);
            return;
        }
        receivedData.erase(receivedData.length() - 2, 2);
        std::map<int, Client*>::iterator it = clients.find(pfd.fd);
        if (it != clients.end()) {
            // cout << buffer;
            process_message(*this, *(it->second), commands, receivedData);
        }
        bzero(buffer, 1024);
    }
}

void Server::process_message(Server &server, Client &sender, std::map<std::string, Command*>& commands, std::string input)
{
	std::string cmd; // nom de la commande.
	std::vector<std::string> args; // arguments de la commande.
    char *token = std::strtok((char *)input.c_str(), " ");
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




