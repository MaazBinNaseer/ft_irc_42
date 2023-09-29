#include "Server.hpp"

Server::Server() {}

void Server::SocketCreation()
{
    /* 
    * AF_INET is for the creation of the socket in the internet domain
    * SOCK_STREAM is for the TCP protocols
    */
    if((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Scoket Creation failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server fd: " << this->server_fd << std::endl;
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
    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(int_port);
    this->address.sin_addr.s_addr = INADDR_ANY;
    if(bind(this->server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("BIND failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Bind: " << this->address.sin_port << " " << this->address.sin_family << std::endl;
}

void Server::ListenSocket()
{
    if (listen(this->server_fd, 3) < 0) 
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Server started, waiting for connections..." << std::endl;
}

void Server::AcceptConnection()
{
    int address_len = sizeof(this->address);
    this->new_socket = accept(this->server_fd, (struct sockaddr *)&address, (socklen_t*)&address_len);
    if(this->new_socket < 0)
    {
        perror("Acceptance Connection has failed");
        exit(EXIT_FAILURE);
    }
    std::cout << "Accpetance Function " << (socklen_t*)&address <<  "New Socket fd" << this->new_socket << std::endl; 
}
Server::~Server() 
{
    if(server_fd != -1) {
        close(server_fd);
    }

    if(new_socket != -1) {
        close(new_socket);
    }
}