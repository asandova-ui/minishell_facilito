#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <sstream> 

class Channel;

class Client {
private:
    int _fd;
    int _id; 
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::string _hostname;
    bool _authenticated;
    std::vector<Channel*> _channels;
    std::string _buffer;
    
    
public:
    Client(int fd);
    ~Client();
    
    // Getters
    int getFd() const;
    const std::string& getNickname() const;
    const std::string& getUsername() const;
    const std::string& getRealname() const;
    const std::string& getHostname() const;
    int getId() const;
    bool isAuthenticated() const;
    
    // Setters
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username, const std::string& realname);
    void setHostname(const std::string& hostname);
    void setAuthenticated(bool authenticated);
    void setId(int id);
    
    // Channel management
    void joinChannel(Channel* channel);
    void leaveChannel(Channel* channel);
    void leaveAllChannels();
    bool isInChannel(Channel* channel) const;
    
    // Buffer management
    void appendToBuffer(const std::string& data);
    bool hasCompleteCommand() const;
    std::string getNextCommand();
    
    // Send messages
    void sendMessage(const std::string& message) const;
    void sendWelcomeMessage();
};

#endif