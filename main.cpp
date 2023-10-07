#include "Server.hpp"

int main(int argc, char **argv)
{
    if(argc > 1 && argc < 4)
    {
        Server a1;
        a1.SocketCreation();
        a1.BindSocket(argv[1]);
        a1.ListenSocket();
        int x = a1.AcceptConnection();
        close(x);
    }
    else
    {
        std::cout << BOLD_RED << "Error: Input should be ./ircserv <port_number> <password>" << RESET << std::endl;
        return (1);
    }


    return (0);
}