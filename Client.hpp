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
	
		int			socket_fd;
		int			client_id;
		std::string	username;
		std::string	nickname;


    public:
        Client();
        ~Client();

};