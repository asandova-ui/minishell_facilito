#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include <poll.h>
#include <string>
#include <sstream> 
#include <map>

class Client;
class Channel;

class Server {
private:
    int _port;
    std::string _password;
    int _serverSocket;
    std::vector<pollfd> _pollFds;
    std::map<int, Client*> _clients;
    std::map<std::string, Channel*> _channels;
    int _clientCounter; 
    
    void setupServer();
    void acceptNewClient();
    void handleClientData(int clientFd);
    void processCommand(Client* client, const std::string& command);
    
public:
    Server(int port, const std::string& password);
    ~Server();
    
    void run();
    void disconnectClient(int clientFd, const std::string& reason);
    Client* findClientByNickname(const std::string& nickname);
    bool isValidNickname(const std::string& nickname);
    
    // Getters
    const std::string& getPassword() const;
    Channel* getChannel(const std::string& name);
    Channel* createChannel(const std::string& name, Client* creator);
    
    // Command handlers
    void handlePass(Client* client, const std::vector<std::string>& args);
    void handleNick(Client* client, const std::vector<std::string>& args);
    void handleUser(Client* client, const std::vector<std::string>& args);
    void handleJoin(Client* client, const std::vector<std::string>& args);
    void handlePrivmsg(Client* client, const std::vector<std::string>& args);
    void handleKick(Client* client, const std::vector<std::string>& args);
    void handleInvite(Client* client, const std::vector<std::string>& args);
    void handleTopic(Client* client, const std::vector<std::string>& args);
    void handleMode(Client* client, const std::vector<std::string>& args);
    void handleQuit(Client* client, const std::vector<std::string>& args);
};

#endif