#include "Server.hpp"

int main()
{
    Server a1;
    a1.SocketCreation();
    a1.BindSocket();
    a1.ListenSocket();
    a1.AcceptConnection();

    return (0);
}