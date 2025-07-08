#include "Server.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream> 

Server* serverPtr = NULL;

void signalHandler(int signum) {
    if (serverPtr != NULL) {
        (void)signum;
        // Limpiar recursos aquí si es necesario
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    
    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        std::cerr << "Invalid port number" << std::endl;
        return 1;
    }
    
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    
    Server server(port, argv[2]);
    serverPtr = &server;
    server.run();
    
    return 0;
}