#ifndef Server_HPP
#define Server_HPP

#define BOLD_RED "\033[1;31m"
#define RESET "\033[0m"

#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <exception>
#include <netinet/in.h>

class Server
{
    private:
        int   server_fd;
        struct sockaddr_in  address;
        struct addrinfo     *res;
        int                 new_socket;
        std::vector<pollfd>    client_fds;

    public:
        char    buffer[256];
        void     SocketCreation();
        void     BindSocket(char *argv);
        void     ListenSocket();
        void     CheckForPort(int port);
        int      AcceptConnection();
        void     HandleClients();
                 Server();
                ~Server();
      

};




#endif
