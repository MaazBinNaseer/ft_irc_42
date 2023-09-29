#include "Server.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    Server a1;
    a1.SocketCreation();
    a1.BindSocket(argv[1]);
    a1.ListenSocket();
    a1.AcceptConnection();

    return (0);
}