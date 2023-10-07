#include "Server.hpp"

int main(int argc, char **argv)
{
    if(argc != 3)  // Modified to specifically check for 3 arguments: program name, port, and password
    {
        std::cout << BOLD_RED << "Error: Input should be ./ircserv <port_number> <password>" << RESET << std::endl;
        return 1;
    }

    Server a1;
    a1.SocketCreation();
    a1.BindSocket(argv[1]);
    a1.ListenSocket();

    // Main event loop
    while (true) {
        a1.AcceptConnection(); // Accept new clients
        a1.HandleClients();   // Handle data from existing clients
        usleep(10000); // Sleep for 10ms
    }



    return (0);
}