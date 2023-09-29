#ifndef Server_HPP
#define Server_HPP


#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <map>
#include <unistd.h>
#include <exception>
#include <netinet/in.h>

class Server
{
    private:
        int                 server_fd;
        struct sockaddr_in  address;
        struct addrinfo     *res;
        int                 new_socket;


    public:
        char    buffer[256];
        void     SocketCreation();
        void     BindSocket(char *argv);
        void     ListenSocket();
        void     CheckForPort(int port);
        void     AcceptConnection();
                 Server();
                ~Server();

};




#endif
