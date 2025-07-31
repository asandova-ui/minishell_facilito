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
#include <numeric>

Server::Server(int port, const std::string& password) : 
    _port(port), _password(password), _serverSocket(-1) {
    setupServer();
}

Server::~Server() {
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        delete it->second;
    }
    _clients.clear();
    
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
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
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
    
    // Log 
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
        if (bytesRead == 0)
            std::cout << "Client " << clientFd << " disconnected" << std::endl;
        else
            std::cerr << "Error reading from client " << clientFd << std::endl;
        disconnectClient(clientFd, "Connection closed");
        return;
    }

    buffer[bytesRead] = '\0';
    
    Client* client = _clients[clientFd];
    if (!client) return;

    client->appendToBuffer(buffer);

    while (_clients.count(clientFd)) {
        if (!client->hasCompleteCommand())
            break;
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
    
    client->sendMessage(":server 001 " + client->getNickname() + " :Password accepted");
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

    exit(0);
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
        client->sendMessage(":server 001 " + client->getNickname() + " :Welcome to the IRC server");
        client->sendMessage(":server 002 " + client->getNickname() + " :Your host is server");
        client->sendMessage(":server 003 " + client->getNickname() + " :This server was created today");
    }
}

void Server::handleJoin(Client* client, const std::vector<std::string>& args) {
    // 1. Verificar autenticación y parámetros
    if (!client->isAuthenticated() || client->getNickname().empty() || client->getUsername().empty()) {
        client->sendMessage(":server 451 " + client->getNickname() + " :You have not registered");
        return;
    }

    if (args.empty()) {
        client->sendMessage(":server 461 " + client->getNickname() + " JOIN :Not enough parameters");
        return;
    }

    // 2. Procesar nombre del canal
    std::string channelName = args[0];
    if (channelName[0] != '#') {
        channelName = "#" + channelName;
    }

    // 3. Buscar o crear el canal
    Channel* channel = getChannel(channelName);
    bool isNewChannel = false;

    if (!channel) {
        channel = createChannel(channelName, client);
        isNewChannel = true;
        
        // El creador ya fue manejado en createChannel, salimos
        return;
    }

    // 4. Verificar si ya es miembro
    if (channel->isMember(client)) {
        return;
    }

    // 5. Verificar restricciones del canal
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

    // 6. Añadir al canal
    channel->addMember(client);
    client->joinChannel(channel);

    // 7. Enviar mensaje JOIN a todos los miembros (incluyéndose a sí mismo)
    std::string joinMsg = ":" + client->getNickname() + "!" + client->getUsername() + "@server JOIN " + channelName;
    channel->broadcast(joinMsg);

    // 8. Enviar información del canal solo al nuevo miembro
    // 8.1. Tema del canal
    if (!channel->getTopic().empty()) {
        client->sendMessage(":server 332 " + client->getNickname() + " " + channelName + " :" + channel->getTopic());
    } else {
        client->sendMessage(":server 331 " + client->getNickname() + " " + channelName + " :No topic is set");
    }

    // 8.2. Lista de usuarios
    std::string userList;
    for (size_t i = 0; i < channel->getMembers().size(); ++i) {
        Client* member = channel->getMembers()[i];
        if (i > 0) userList += " ";
        if (channel->isOperator(member)) userList += "@";
        userList += member->getNickname();
    }
    client->sendMessage(":server 353 " + client->getNickname() + " = " + channelName + " :" + userList);
    client->sendMessage(":server 366 " + client->getNickname() + " " + channelName + " :End of /NAMES list.");

    // 9. Si el canal es nuevo, notificar modo operador
    if (isNewChannel) {
        channel->addOperator(client);
        std::string modeMsg = ":server MODE " + channelName + " +o " + client->getNickname();
        channel->broadcast(modeMsg);
    }
}

void Server::handlePrivmsg(Client* client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        client->sendMessage(":server 461 PRIVMSG :Not enough parameters");
        return;
    }

    std::string target = args[0];
    std::string message = args[1];

    if (target[0] == '#') {
        Channel* channel = getChannel(target);
        if (!channel) {
            client->sendMessage(":server 403 " + client->getNickname() + " " + target + " :No such channel");
            return;
        }
        if (!channel->isMember(client)) {
            client->sendMessage(":server 442 " + client->getNickname() + " " + target + " :You're not on that channel");
            return;
        }
        
        // Formato CORRECTO para HexChat
        std::string privmsg = ":" + client->getNickname() + "!" + client->getUsername() + "@server PRIVMSG " + target + " :" + message;
        channel->broadcast(privmsg, client);
    } else {
        // Mensaje privado
        Client* targetClient = findClientByNickname(target);
        if (!targetClient) {
            client->sendMessage(":server 401 " + client->getNickname() + " " + target + " :No such nick");
            return;
        }
        targetClient->sendMessage(":" + client->getNickname() + "!" + client->getUsername() + "@server PRIVMSG " + target + " :" + message);
    }
}

void Server::handleKick(Client* client, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        client->sendMessage(":server 461 KICK :Not enough parameters");
        return;
    }

    std::string channelName = args[0];
    std::string targetNick = args[1];
    std::string reason = args.size() > 2 ? std::accumulate(args.begin() + 2, args.end(), std::string(" ")) : "No reason given";

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
    
    // Añadir al creador como miembro PRIMERO
    channel->addMember(creator);
    creator->joinChannel(channel);
    
    // Luego hacerlo operador
    channel->addOperator(creator);
    
    // Enviar confirmaciones al creador
    std::string joinMsg = ":" + creator->getNickname() + "!" + creator->getUsername() + "@server JOIN " + name;
    creator->sendMessage(joinMsg);
    
    creator->sendMessage(":server 331 " + creator->getNickname() + " " + name + " :No topic is set");
    creator->sendMessage(":server 353 " + creator->getNickname() + " = " + name + " :@" + creator->getNickname());
    creator->sendMessage(":server 366 " + creator->getNickname() + " " + name + " :End of /NAMES list.");
    creator->sendMessage(":server MODE " + name + " +o " + creator->getNickname());
    
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
