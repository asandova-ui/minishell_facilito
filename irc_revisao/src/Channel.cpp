#include "Channel.hpp"
#include "Client.hpp"
#include <algorithm>

Channel::Channel(const std::string& name) : _name(name), _inviteOnly(false), 
    _topicRestricted(false), _userLimit(0) {}

Channel::~Channel() {}

void Channel::broadcast(const std::string& message, Client* sender) const {
    for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
        if (*it != sender) {
            (*it)->sendMessage(message);
        }
    }
}

bool Channel::isInviteOnly() const {
    return _inviteOnly;
}

bool Channel::isTopicRestricted() const {
    return _topicRestricted;
}

void Channel::setTopicRestricted(bool restricted) {
    _topicRestricted = restricted;
}

bool Channel::isInvited(const std::string& nickname) const {
    for (size_t i = 0; i < _invited.size(); ++i) {
        if (_invited[i] == nickname) {
            return true;
        }
    }
    return false;
}

void Channel::setInviteOnly(bool inviteOnly) {
    _inviteOnly = inviteOnly;
}

void Channel::addInvited(const std::string& nickname) {
    if (!isInvited(nickname)) {
        _invited.push_back(nickname);
    }
}

void Channel::broadcastToOthers(const std::string& message, Client* sender) const {
    if (!sender) return;
    broadcast(message, sender);
}

void Channel::addMember(Client* client) {
    if (!isMember(client)) {
        _members.push_back(client);
    }
}

void Channel::addOperator(Client* client) {
    if (!isOperator(client)) {
        _operators.push_back(client);
    }
}

int Channel::getUserLimit() const {
    return _userLimit;
}

void Channel::setUserLimit(int limit) {
    _userLimit = limit;
}

const std::string& Channel::getKey() const {
    return _key;
}

void Channel::setKey(const std::string& key) {
    _key = key;
}

const std::string& Channel::getName() const {
    return _name;
}

void Channel::setTopic(const std::string& topic) {
    _topic = topic;
}

bool Channel::isFull() const {
    return (_userLimit > 0 && _members.size() >= static_cast<size_t>(_userLimit));
}

bool Channel::isMember(Client* client) const {
    for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
        if (*it == client) {
            return true;
        }
    }
    return false;
}

bool Channel::isOperator(Client* client) const {
    for (size_t i = 0; i < _operators.size(); ++i) {
        if (_operators[i] == client) {
            return true;
        }
    }
    return false;
}

void Channel::removeMember(Client* client) {
    for (size_t i = 0; i < _members.size(); ++i) {
        if (_members[i] == client) {
            _members.erase(_members.begin() + i);
            break;
        }
    }
    removeOperator(client);
}

void Channel::removeOperator(Client* client) {
    for (size_t i = 0; i < _operators.size(); ++i) {
        if (_operators[i] == client) {
            _operators.erase(_operators.begin() + i);
            break;
        }
    }
}

void Channel::sendNamesList(Client* client) const {
    std::string namesList = ":server 353 " + client->getNickname() + " = " + _name + " :";
    
    for (std::vector<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
        if (it != _members.begin()) {
            namesList += " ";
        }
        if (isOperator(*it)) {
            namesList += "@";
        }
        namesList += (*it)->getNickname();
    }
    
    client->sendMessage(namesList);
}

const std::string& Channel::getTopic() const {
    return _topic;
}