#include "Client.hpp"
#include "Channel.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

Client::Client(int fd) : _fd(fd), _authenticated(false) {}

Client::~Client() {
    if (_fd != -1) {
        close(_fd);
    }
}

void Client::setUsername(const std::string& username, const std::string& realname) {
    _username = username;
    _realname = realname;
}

void Client::joinChannel(Channel* channel) {
    _channels.push_back(channel);
}

void Client::setHostname(const std::string& hostname) {
    _hostname = hostname;
}

void Client::leaveAllChannels() {
    for (size_t i = 0; i < _channels.size(); ++i) {
        _channels[i]->removeMember(this);
        _channels[i]->broadcast(":" + getNickname() + "!" + getUsername() + "@localhost QUIT :Disconnected");
    }
    _channels.clear();
}

bool Client::isAuthenticated() const {
    return _authenticated;
}

const std::string& Client::getUsername() const {
    return _username;
}

void Client::setAuthenticated(bool status) {
    _authenticated = status;
}

const std::string& Client::getNickname() const {
    return _nickname;
}

void Client::setNickname(const std::string& nickname) {
    _nickname = nickname;
}

int Client::getFd() const {
    return _fd;
}

void Client::setId(int id) {
    _id = id;
}

int Client::getId() const {
    return _id;
}

const std::string& Client::getHostname() const {
    return _hostname;
}

void Client::sendMessage(const std::string& message) const {
    if (_fd == -1) return;
    
    std::string fullMessage = message + "\r\n";
    ssize_t bytesSent = send(_fd, fullMessage.c_str(), fullMessage.length(), 0);
    if (bytesSent <= 0) {
        std::cerr << "Error sending message to client " << _fd << std::endl;
    }
}

void Client::sendWelcomeMessage() {
    std::string welcome = ":Welcome to the IRC server " + getNickname();
    sendMessage(welcome);
    
    sendMessage(":" + getNickname() + ": This server was created now");
}

void Client::appendToBuffer(const std::string& data) {
    _buffer += data;
}

bool Client::hasCompleteCommand() const {
    return _buffer.find("\r\n") != std::string::npos || 
           _buffer.find("\n") != std::string::npos;
}

std::string Client::getNextCommand() {
    size_t pos = _buffer.find("\r\n");
    if (pos == std::string::npos) {
        pos = _buffer.find("\n");
        if (pos == std::string::npos) return "";
    }
    
    std::string command = _buffer.substr(0, pos);
    _buffer.erase(0, pos + (pos < _buffer.length() - 1 && _buffer[pos] == '\r' ? 2 : 1));
    return command;
}

bool Client::isInChannel(Channel* channel) const {
    for (size_t i = 0; i < _channels.size(); ++i) {
        if (_channels[i] == channel) {
            return true;
        }
    }
    return false;
}

void Client::leaveChannel(Channel* channel) {
    for (size_t i = 0; i < _channels.size(); ++i) {
        if (_channels[i] == channel) {
            _channels.erase(_channels.begin() + i);
            channel->removeMember(this);
            break;
        }
    }
}