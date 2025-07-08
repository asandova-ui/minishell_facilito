#include "Utils.hpp"
#include <sstream>
#include <cctype>

std::string toUpperCase(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = toupper(result[i]);
    }
    return result;
}

std::vector<std::string> splitCommand(const std::string& command) {
    std::vector<std::string> args;
    std::istringstream iss(command);
    std::string arg;
    
    while (iss >> arg) {
        // Eliminar los dos puntos del primer parámetro si es un prefijo
        if (args.empty() && !arg.empty() && arg[0] == ':') {
            arg = arg.substr(1);
        }
        args.push_back(arg);
    }
    
    // Si el último argumento comienza con :, combinar los restantes
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i][0] == ':' && i != 0) {
            std::string combined;
            for (size_t j = i; j < args.size(); ++j) {
                if (j != i) combined += " ";
                combined += args[j].substr(j == i ? 1 : 0);
            }
            args.resize(i);
            args.push_back(combined);
            break;
        }
    }
    
    return args;
}

bool isValidNickname(const std::string& nickname) {
    if (nickname.empty() || nickname.length() > 9) return false;
    
    const std::string special = "[]\\`_^{|}";
    for (size_t i = 0; i < nickname.length(); ++i) {
        char c = nickname[i];
        if (!isalpha(c) && !isdigit(c) && special.find(c) == std::string::npos) {
            return false;
        }
    }
    
    return true;
}