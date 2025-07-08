#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fcntl.h> 

Server::Server(int port, const std::string& password) : 
    _port(port), _password(password), _serverSocket(-1) {
    setupServer();
}

Server::~Server() {
    // Limpiar todos los clientes
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        delete it->second;
    }
    _clients.clear();
    
    // Limpiar todos los canales
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        delete it->second;
    }
    _channels.clear();
    
    if (_serverSocket != -1) {
        close(_serverSocket);
    }
}

void Server::setupServer() {
    // Crear socket
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1) {
        throw std::runtime_error("Error creating socket");
    }
    
    // Configurar opciones del socket
    int opt = 1;
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        close(_serverSocket);
        throw std::runtime_error("Error setting socket options");
    }
    
    // Configurar dirección del servidor
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);
    
    // Vincular socket
    if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        close(_serverSocket);
        throw std::runtime_error("Error binding socket");
    }
    
    // Escuchar conexiones
    if (listen(_serverSocket, SOMAXCONN) == -1) {
        close(_serverSocket);
        throw std::runtime_error("Error listening on socket");
    }
    
    // Configurar pollfd para el servidor
    pollfd serverPollFd;
    serverPollFd.fd = _serverSocket;
    serverPollFd.events = POLLIN;
    serverPollFd.revents = 0;
    _pollFds.push_back(serverPollFd);
    
    std::cout << "Server listening on port " << _port << std::endl;
}

void Server::run() {
    while (true) {
        int pollResult = poll(&_pollFds[0], _pollFds.size(), -1);
        if (pollResult == -1) {
            throw std::runtime_error("Error in poll");
        }
        
        for (size_t i = 0; i < _pollFds.size(); ++i) {
            if (_pollFds[i].revents & POLLIN) {
                if (_pollFds[i].fd == _serverSocket) {
                    acceptNewClient();
                } else {
                    handleClientData(_pollFds[i].fd);
                }
            }
        }
    }
}

void Server::acceptNewClient() {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientFd = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientFd == -1) {
        std::cerr << "Error accepting new client" << std::endl;
        return;
    }
    
    // Configurar el socket como no bloqueante
    int flags = fcntl(clientFd, F_GETFL, 0);
    if (fcntl(clientFd, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Error setting non-blocking mode for client " << clientFd << std::endl;
        close(clientFd);
        return;
    }
    
    // Crear nuevo cliente con ID lógico
    static int clientIdCounter = 1;  // Contador estático para IDs lógicos
    Client* newClient = new Client(clientFd);
    newClient->setHostname(inet_ntoa(clientAddr.sin_addr));
    newClient->setId(clientIdCounter++);  // Asignar ID lógico secuencial
    
    // Almacenar cliente
    _clients[clientFd] = newClient;
    
    // Añadir a poll
    pollfd clientPollFd;
    clientPollFd.fd = clientFd;
    clientPollFd.events = POLLIN | POLLOUT;
    clientPollFd.revents = 0;
    _pollFds.push_back(clientPollFd);
    
    // Log mejorado
    std::cout << "New client connected: " 
              << "ID=" << newClient->getId() 
              << ", FD=" << clientFd 
              << ", IP=" << inet_ntoa(clientAddr.sin_addr)  // Usamos la IP directamente aquí
              << std::endl;
              
    // Mensaje de bienvenida
    newClient->sendMessage("Welcome to the IRC server! Please authenticate with PASS, NICK, and USER commands.");
}
void Server::handleClientData(int clientFd) {
    char buffer[512];
    ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesRead <= 0) {
        if (bytesRead == 0) {
            std::cout << "Client " << clientFd << " disconnected" << std::endl;
        } else {
            std::cerr << "Error reading from client " << clientFd << std::endl;
        }
        disconnectClient(clientFd, "Connection closed");
        return;
    }
    
    buffer[bytesRead] = '\0';
    Client* client = _clients[clientFd];
    client->appendToBuffer(buffer);
    
    while (client->hasCompleteCommand()) {
        std::string command = client->getNextCommand();
        processCommand(client, command);
    }
}

void Server::processCommand(Client* client, const std::string& command) {
    std::vector<std::string> args = splitCommand(command);
    if (args.empty()) return;
    
    std::string cmd = toUpperCase(args[0]);
    args.erase(args.begin());
    
    if (cmd == "PASS") handlePass(client, args);
    else if (cmd == "NICK") handleNick(client, args);
    else if (cmd == "USER") handleUser(client, args);
    else if (!client->isAuthenticated()) {
        client->sendMessage(":server 451 * :You have not registered");
        return;
    }
    else if (cmd == "JOIN") handleJoin(client, args);
    else if (cmd == "PRIVMSG") handlePrivmsg(client, args);
    else if (cmd == "KICK") handleKick(client, args);
    else if (cmd == "INVITE") handleInvite(client, args);
    else if (cmd == "TOPIC") handleTopic(client, args);
    else if (cmd == "MODE") handleMode(client, args);
    else if (cmd == "QUIT") handleQuit(client, args);
    else {
        client->sendMessage(":server 421 " + client->getNickname() + " " + cmd + " :Unknown command");
    }
}

// Implementaciones de los comandos (ejemplo con algunos comandos)

void Server::handlePass(Client* client, const std::vector<std::string>& args) {
    if (client->isAuthenticated()) {
        client->sendMessage(":server 462 :You may not reregister");
        return;
    }
    
    if (args.empty() || args[0] != _password) {
        client->sendMessage(":server 464 :Password incorrect");
        disconnectClient(client->getFd(), "Bad password");
        return;
    }
    
    client->sendMessage(":server 001 :Password accepted");
}

bool Server::isValidNickname(const std::string& nickname) {
    if (nickname.empty() || nickname.size() > 15) return false;
    
    const std::string special = "[]\\`_^{|}";
    for (size_t i = 0; i < nickname.size(); ++i) {
        char c = nickname[i];
        if (!isalpha(c) && !isdigit(c) && special.find(c) == std::string::npos) {
            return false;
        }
    }
    return true;
}

void Server::handleNick(Client* client, const std::vector<std::string>& args) {
    if (args.empty()) {
        client->sendMessage(":server 431 :No nickname given");
        return;
    }
    
    std::string newNick = args[0];
    std::string oldNick = client->getNickname();
    
    // Validar el nickname
    if (!isValidNickname(newNick)) {
        client->sendMessage(":server 432 " + oldNick + " " + newNick + " :Erroneous nickname");
        return;
    }
    
    // Verificar si el nickname ya está en uso
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second != client && it->second->getNickname() == newNick) {
            client->sendMessage(":server 433 " + oldNick + " " + newNick + " :Nickname is already in use");
            return;
        }
    }
    
    // Cambiar el nickname
    client->setNickname(newNick);
    
    // Notificar a otros clientes si ya está registrado
    if (client->isAuthenticated()) {
        // Notificar a todos los canales donde está presente
        for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
            if (it->second->isMember(client)) {
                it->second->broadcast(":" + oldNick + "!" + client->getUsername() + "@localhost NICK :" + newNick);
            }
        }
    }
}

// Implementar otros comandos de manera similar...

void Server::disconnectClient(int clientFd, const std::string& reason) {
    Client* client = _clients[clientFd];
    if (!client) return;
    
    // Notificar a los canales que el cliente ha salido
    client->leaveAllChannels();
    
    // Eliminar de poll
    for (std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); ++it) {
        if (it->fd == clientFd) {
            _pollFds.erase(it);
            break;
        }
    }
    
    // Cerrar socket y eliminar cliente
    close(clientFd);
    _clients.erase(clientFd);
    delete client;
    
    std::cout << "Client " << clientFd << " disconnected: " << reason << std::endl;
}

void Server::handleUser(Client* client, const std::vector<std::string>& args) {
    if (args.size() < 4) {
        client->sendMessage(":server 461 USER :Not enough parameters");
        return;
    }
    
    client->setUsername(args[0], args[3]);
    client->setAuthenticated(true);
    
    // Si ya tiene nickname, completar registro
    if (!client->getNickname().empty()) {
        client->sendWelcomeMessage();
    }
}

void Server::handleJoin(Client* client, const std::vector<std::string>& args) {
    if (!client->isAuthenticated() || client->getNickname().empty() || client->getUsername().empty()) {
        client->sendMessage(":server 451 :You have not registered");
        return;
    }

    if (args.empty()) {
        client->sendMessage(":server 461 JOIN :Not enough parameters");
        return;
    }
    
    std::string channelName = args[0];
    if (channelName[0] != '#') {
        channelName = "#" + channelName;
    }
    
    Channel* channel = getChannel(channelName);
    if (!channel) {
        channel = createChannel(channelName, client);
        std::string joinMsg = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost JOIN " + channelName;
        client->sendMessage(joinMsg);
        channel->sendNamesList(client);
        return;
    }
    
    if (channel->isMember(client)) {
        return;
    }
    
    // Verificación de modos de canal
    if (channel->isInviteOnly() && !channel->isInvited(client->getNickname())) {
        client->sendMessage(":server 473 " + client->getNickname() + " " + channelName + " :Cannot join channel (+i)");
        return;
    }
    
    if (!channel->getKey().empty() && (args.size() < 2 || args[1] != channel->getKey())) {
        client->sendMessage(":server 475 " + client->getNickname() + " " + channelName + " :Cannot join channel (+k)");
        return;
    }
    
    if (channel->isFull()) {
        client->sendMessage(":server 471 " + client->getNickname() + " " + channelName + " :Cannot join channel (+l)");
        return;
    }
    
    channel->addMember(client);
    client->joinChannel(channel);
    std::string joinMsg = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost JOIN " + channelName;
    channel->broadcast(joinMsg);
    channel->sendNamesList(client);
    
    if (!channel->getTopic().empty()) {
        client->sendMessage(":server 332 " + client->getNickname() + " " + channelName + " :" + channel->getTopic());
    }
}

// Implementaciones básicas para los otros comandos
void Server::handlePrivmsg(Client* client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        client->sendMessage(":server 461 PRIVMSG :Not enough parameters");
        return;
    }

    std::string target = args[0];
    std::string message = args[1];

    if (target[0] == '#') {
        // Mensaje a canal
        Channel* channel = getChannel(target);
        if (!channel) {
            client->sendMessage(":server 403 " + client->getNickname() + " " + target + " :No such channel");
            return;
        }
        if (!channel->isMember(client)) {
            client->sendMessage(":server 442 " + client->getNickname() + " " + target + " :You're not on that channel");
            return;
        }
        channel->broadcast(":" + client->getNickname() + "!" + client->getUsername() + "@localhost PRIVMSG " + target + " :" + message, client);
    } else {
        // Mensaje privado a usuario
        Client* targetClient = findClientByNickname(target);
        if (!targetClient) {
            client->sendMessage(":server 401 " + client->getNickname() + " " + target + " :No such nick");
            return;
        }
        targetClient->sendMessage(":" + client->getNickname() + "!" + client->getUsername() + "@localhost PRIVMSG " + target + " :" + message);
    }
}

void Server::handleKick(Client* client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        client->sendMessage(":server 461 KICK :Not enough parameters");
        return;
    }

    std::string channelName = args[0];
    std::string targetNick = args[1];
    std::string reason = args.size() > 2 ? args[2] : "No reason given";

    Channel* channel = getChannel(channelName);
    if (!channel) {
        client->sendMessage(":server 403 " + client->getNickname() + " " + channelName + " :No such channel");
        return;
    }

    if (!channel->isOperator(client)) {
        client->sendMessage(":server 482 " + client->getNickname() + " " + channelName + " :You're not channel operator");
        return;
    }

    Client* target = findClientByNickname(targetNick);
    if (!target || !channel->isMember(target)) {
        client->sendMessage(":server 441 " + client->getNickname() + " " + targetNick + " " + channelName + " :They aren't on that channel");
        return;
    }

    // Notificar a todos en el canal
    channel->broadcast(":" + client->getNickname() + "!" + client->getUsername() + "@localhost KICK " + channelName + " " + targetNick + " :" + reason);
    
    // Eliminar al usuario del canal
    channel->removeMember(target);
    target->leaveChannel(channel);
}

void Server::handleInvite(Client* client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        client->sendMessage(":server 461 INVITE :Not enough parameters");
        return;
    }

    std::string targetNick = args[0];
    std::string channelName = args[1];

    Channel* channel = getChannel(channelName);
    if (!channel) {
        client->sendMessage(":server 403 " + client->getNickname() + " " + channelName + " :No such channel");
        return;
    }

    if (!channel->isOperator(client)) {
        client->sendMessage(":server 482 " + client->getNickname() + " " + channelName + " :You're not channel operator");
        return;
    }

    Client* target = findClientByNickname(targetNick);
    if (!target) {
        client->sendMessage(":server 401 " + client->getNickname() + " " + targetNick + " :No such nick");
        return;
    }

    if (channel->isMember(target)) {
        client->sendMessage(":server 443 " + client->getNickname() + " " + targetNick + " " + channelName + " :is already on channel");
        return;
    }

    // Añadir a la lista de invitados y notificar
    channel->addInvited(targetNick);
    target->sendMessage(":" + client->getNickname() + "!" + client->getUsername() + "@localhost INVITE " + targetNick + " :" + channelName);
    client->sendMessage(":server 341 " + client->getNickname() + " " + targetNick + " " + channelName);
}

void Server::handleTopic(Client* client, const std::vector<std::string>& args) {
    if (args.empty()) {
        client->sendMessage(":server 461 TOPIC :Not enough parameters");
        return;
    }

    std::string channelName = args[0];
    Channel* channel = getChannel(channelName);
    if (!channel) {
        client->sendMessage(":server 403 " + client->getNickname() + " " + channelName + " :No such channel");
        return;
    }

    if (!channel->isMember(client)) {
        client->sendMessage(":server 442 " + client->getNickname() + " " + channelName + " :You're not on that channel");
        return;
    }

    // Mostrar topic actual
    if (args.size() == 1) {
        if (channel->getTopic().empty()) {
            client->sendMessage(":server 331 " + client->getNickname() + " " + channelName + " :No topic is set");
        } else {
            client->sendMessage(":server 332 " + client->getNickname() + " " + channelName + " :" + channel->getTopic());
        }
        return;
    }

    // Cambiar topic
    if (channel->isTopicRestricted() && !channel->isOperator(client)) {
        client->sendMessage(":server 482 " + client->getNickname() + " " + channelName + " :You're not channel operator");
        return;
    }

    std::string newTopic = args[1];
    channel->setTopic(newTopic);
    channel->broadcast(":" + client->getNickname() + "!" + client->getUsername() + "@localhost TOPIC " + channelName + " :" + newTopic);
}

void Server::handleMode(Client* client, const std::vector<std::string>& args) {
    if (args.size() < 1) {
        client->sendMessage(":server 461 MODE :Not enough parameters");
        return;
    }

    std::string channelName = args[0];  // Cambiado de 'target' a 'channelName'
    
    // Modo de usuario (no implementado)
    if (channelName[0] != '#') {
        client->sendMessage(":server 502 " + client->getNickname() + " :Cannot change mode for other users");
        return;
    }

    Channel* channel = getChannel(channelName);
    if (!channel) {
        client->sendMessage(":server 403 " + client->getNickname() + " " + channelName + " :No such channel");
        return;
    }

    // Mostrar modos actuales
    if (args.size() == 1) {
        std::string modes = "+";
        std::string modeParams;
        
        if (channel->isInviteOnly()) modes += "i";
        if (!channel->getKey().empty()) {
            modes += "k";
            modeParams += " " + channel->getKey();
        }
        if (channel->getUserLimit() > 0) {
            modes += "l";
            std::ostringstream oss;
            oss << channel->getUserLimit();
            modeParams += " " + oss.str();
        }
        
        client->sendMessage(":server 324 " + client->getNickname() + " " + channelName + " " + modes + modeParams);
        return;
    }

    // Cambiar modos
    if (!channel->isOperator(client)) {
        client->sendMessage(":server 482 " + client->getNickname() + " " + channelName + " :You're not channel operator");
        return;
    }

    std::string mode = args[1];
    bool setMode = (mode[0] == '+');
    char modeChar = mode[1];

    Client* targetClient = NULL;  // Para modo +o/-o
    
    switch (modeChar) {
        case 'i': // Invite-only
            channel->setInviteOnly(setMode);
            break;
        case 't': // Topic restriction
            channel->setTopicRestricted(setMode);
            break;
        case 'k': // Channel key
            if (setMode) {
                if (args.size() < 3) {
                    client->sendMessage(":server 461 MODE :Not enough parameters");
                    return;
                }
                channel->setKey(args[2]);
            } else {
                channel->setKey("");
            }
            break;
        case 'o': // Operator privilege
            if (args.size() < 3) {
                client->sendMessage(":server 461 MODE :Not enough parameters");
                return;
            }
            targetClient = findClientByNickname(args[2]);
            if (!targetClient || !channel->isMember(targetClient)) {
                client->sendMessage(":server 441 " + client->getNickname() + " " + args[2] + " " + channelName + " :They aren't on that channel");
                return;
            }
            if (setMode) {
                channel->addOperator(targetClient);
            } else {
                channel->removeOperator(targetClient);
            }
            break;
        case 'l': // User limit
            if (setMode) {
                if (args.size() < 3) {
                    client->sendMessage(":server 461 MODE :Not enough parameters");
                    return;
                }
                channel->setUserLimit(atoi(args[2].c_str()));
            } else {
                channel->setUserLimit(0);
            }
            break;
        default:
            client->sendMessage(":server 472 " + client->getNickname() + " " + std::string(1, modeChar) + " :is unknown mode char");
            return;
    }

    // Notificar el cambio
    std::string modeChange = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost MODE " + channelName + " " + mode;
    if (args.size() > 2) {
        modeChange += " " + args[2];
    }
    channel->broadcast(modeChange);
}

void Server::handleQuit(Client* client, const std::vector<std::string>& args) {
    std::string reason = args.empty() ? "Client quit" : args[0];
    disconnectClient(client->getFd(), reason);
}

Channel* Server::getChannel(const std::string& name) {
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it != _channels.end()) {
        return it->second;
    }
    return NULL;
}

Channel* Server::createChannel(const std::string& name, Client* creator) {
    Channel* channel = new Channel(name);
    _channels[name] = channel;
    channel->addMember(creator);
    channel->addOperator(creator);
    creator->sendMessage(":" + creator->getNickname() + " MODE " + name + " +o " + creator->getNickname());
    return channel;
}

Client* Server::findClientByNickname(const std::string& nickname) {
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it->second->getNickname() == nickname) {
            return it->second;
        }
    }
    return NULL;
}
