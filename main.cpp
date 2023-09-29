#include "Server.hpp"

int main(int argc, char **argv)
{
    // (void)argc;
    // Server a1;
    // a1.SocketCreation();
    // a1.BindSocket(argv[1]);
    // a1.ListenSocket();
    // a1.AcceptConnection();

    std::vector<Server> servers;

    for (int i = 1; i < argc; i++) 
    {
        Server serverInstance;

        serverInstance.SocketCreation();
        serverInstance.BindSocket(argv[i]);
        serverInstance.ListenSocket();

        servers.push_back(serverInstance);

        serverInstance.printDetails();
    }

    while(true) //* Right now accepting one 
    {
        servers[0].AcceptConnection();
    }

    return (0);
}