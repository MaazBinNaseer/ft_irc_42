/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFuncs.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:31:31 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/18 21:24:50 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

int	Server::appendpollfd(int new_socket)
{
	struct pollfd mypoll;

	std::memset(&mypoll, 0, sizeof(mypoll));
	mypoll.fd = new_socket;
	mypoll.events = POLLIN;
	mypoll.revents = 0;
	this->clientfds.push_back(mypoll);

	return (new_socket);
}

int	Server::assign(char *portstr, char *pass)
{
	std::stringstream stream(portstr);
	unsigned int	port;
	stream >> port;
	if (port < MINPORT || port > MAXPORT)
		return (std::cerr << RED "Invalid port number! Choose a port between "
			<< MINPORT << " and " << MAXPORT << "!\n" RESET, 1);

	this->sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sfd == -1)
		return (std::cerr << RED "Socket Failed!" RESET "\n", 1);

	std::memset(&this->addr, 0, sizeof(this->addr));
	this->addr.sin_family = AF_INET;
	this->addr.sin_port = htons(port);
	this->addr.sin_addr.s_addr = INADDR_ANY;
	
	appendpollfd(this->sfd);

	this->password = pass;

	return (0);
}

int	Server::accept_connect( void )
{
	if (poll(this->clientfds.data(), this->clientfds.size(), 10) == -1)
		return (std::cerr << RED "Poll Failed!" RESET "\n", -1);

	if (!(this->clientfds[0].revents & POLLIN))
		return (0);
	
	struct sockaddr_in	caddr;
	size_t addlen = sizeof(caddr);

	int cfd = accept(this->sfd, (struct sockaddr *) &caddr, (socklen_t *) &addlen);
	if (cfd == -1)
		return (std::cerr << RED "Accept Failed!" RESET "\n", -1);
	Client	login = Client(cfd, inet_ntoa(caddr.sin_addr));
	this->clients.insert(std::pair<int, Client>(cfd, login));
	std::cout << GREEN "New Connection! Socket " << cfd << RESET "\n";
	return (appendpollfd(cfd));
}

//TODO Figure out and handle receiving authentications from the client: "Irssi"
void	Server::HandleParse(int i)
{
	std::string buffed;

	buffed = this->clients[clientfds[i].fd].getReceiveBuffer();
	if (buffed.find('\n') != std::string::npos)
	{
		Commands  extract(&this->clients[this->clientfds[i].fd], this);
		extract.trim(buffed);
		extract.assignCommand(buffed);
		if (!buffed.empty())
			extract.assignArguments(buffed);
		extract.executeCommand();
		this->clients[this->clientfds[i].fd].getReceiveBuffer().clear();
	}
}

// Handling the client data would be similar to the HandleClients function from the previous version
int Server::HandleClients()
{
	char buffer[2056];
	int	valread;
	
	for (size_t i = 1; i < clientfds.size(); i++)
	{
		if (this->clientfds[i].revents & POLLIN) // ! CHECK IF & OR &&
		{
		
			valread = recv(this->clientfds[i].fd, buffer, sizeof(buffer), 0);
			std::memset(buffer + valread, 0, 1024 - valread);
			if (valread < 0)
				return (-1);
			else if (valread == 0)
			{
				// ! DELETE ALL INSTANCES OF THIS SOCKET_FD ACROSS ANY CHANNEL
				close(this->clientfds[i].fd);
				this->clientfds.erase(this->clientfds.begin() + i--);
			}
			else if (buffer[0] != '\n')
			{
				this->clients[clientfds[i].fd].getReceiveBuffer() += buffer;
				HandleParse(i);
			}		
		}
	}
	return (0);
}

int	Server::bootup(char	*portstr, char *pass)
{
	if (assign(portstr, pass))
		return (1);

	if (bind(this->sfd, (struct sockaddr *) &this->addr, sizeof(this->addr)))
		return (std::cerr << RED "Bind Failed!" RESET "\n", 1);
	
	if (listen(this->sfd, 3))
		return (std::cerr << RED "Listen Failed!" RESET "\n", 1);
 
	std::cout << GREEN "Server Started! Welcoming Clients!" RESET "\n";
	
	while (true)
		if (accept_connect() == -1 || HandleClients() == -1)
			return (1);
	return (0);
}
