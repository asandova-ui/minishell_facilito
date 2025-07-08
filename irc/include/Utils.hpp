#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <sstream> 

std::string toUpperCase(const std::string& str);
std::vector<std::string> splitCommand(const std::string& command);
bool isValidNickname(const std::string& nickname);

#endif