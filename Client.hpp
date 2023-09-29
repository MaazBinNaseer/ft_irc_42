#pragma once 

#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <map>
#include <exception>
#include <netinet/in.h>

class Client
{
    private: 
        int     client_fd;
        int     new_socket;
        sockaddr_in server_address; 

    public:
        Client();
        ~Client();

};