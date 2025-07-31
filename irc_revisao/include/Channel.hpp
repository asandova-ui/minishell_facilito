#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>


class Client;

class Channel {
private:
    std::string _name;
    std::string _topic;
    std::string _topicSetter;
    time_t _topicTime;
    std::string _key;
    std::vector<Client*> _members;
    std::vector<Client*> _operators;
    std::vector<std::string> _invited;
    bool _inviteOnly;
    bool _topicRestricted;
    int _userLimit;
    
public:
    Channel(const std::string& name);
    ~Channel();
    
    // Getters
    const std::string& getName() const;
    const std::string& getTopic() const;
    size_t getMemberCount() const;
    const std::string& getKey() const;
    int getUserLimit() const;


    // Setters
    void setTopic(const std::string& topic);
    void setUserLimit(int limit);
    
    // Member management
    void addMember(Client* client);
    void removeMember(Client* client);
    bool isMember(Client* client) const;
    bool isOperator(Client* client) const;
    void sendNamesList(Client* client) const;
    
    // Operator management
    void addOperator(Client* client);
    void removeOperator(Client* client);
    
    // Invite management
    void addInvited(const std::string& nickname);
    bool isInvited(const std::string& nickname) const;
    
    // Mode management
    void setInviteOnly(bool inviteOnly);
    bool isInviteOnly() const;
    void setTopicRestricted(bool restricted);
    bool isTopicRestricted() const;
    void setKey(const std::string& key);
    bool checkKey(const std::string& key) const;
    bool isFull() const;
    

    std::string getTopicSetter() const;
    time_t getTopicTimestamp() const;
    void setTopic(const std::string& topic, const std::string& setter);
    std::string getFormattedUserList() const;
    
    // Messaging
    void broadcast(const std::string& message, Client* sender = NULL) const;
    void broadcastToOthers(const std::string& message, Client* sender) const;
};

#endif