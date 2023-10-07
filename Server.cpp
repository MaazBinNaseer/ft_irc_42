#include "Server.hpp"

Server::Server() {}

void Server::SocketCreation()
{
    /* 
    * AF_INET is for the creation of the socket in the internet domain
    * SOCK_STREAM is for the TCP protocols
    */
    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd  == -1)
    {
        perror("Scoket Creation failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server fd: " << server_fd << std::endl;
}

void Server::CheckForPort(int port)
{
      if (port < 1024 || port > 65535) 
      {
        std::cout << "Error: Invalid port number. Choose a port between 1025 and 65535 " << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::BindSocket(char *argv)
{
    std::stringstream stream(argv);

    int int_port;
    stream >> int_port;

    this->CheckForPort(int_port);

    // Clear the structure
    memset(&this->address, 0, sizeof(this->address));

    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(int_port);
    this->address.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
    {
        perror("BIND failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Bind: " << ntohs(this->address.sin_port) << " " << this->address.sin_family << std::endl;
}

void Server::ListenSocket()
{
    if(listen(server_fd, 3) < 0) 
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }   
    std::cout << "Server started, waiting for connections..." << std::endl;
}

int Server::AcceptConnection()
{
    struct sockaddr_in client_address;
        int address_len = sizeof(client_address);

        int new_sock = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&address_len);
        if(new_sock < 0) {
            perror("Acceptance connection has failed");
            exit(EXIT_FAILURE);
        }
        
        std::cout << "Accepted connection on socket: " << server_fd << ", New socket fd: " << new_sock << std::endl;
        return new_sock;

    // Optionally, you can store new_sock somewhere if you plan on communicating with the client later
}
Server::~Server() 
{}