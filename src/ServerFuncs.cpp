/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFuncs.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:31:31 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/25 20:01:08 by mgoltay          ###   ########.fr       */
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
		throw FailedFunction("Socket");

	std::memset(&this->addr, 0, sizeof(this->addr));
	this->addr.sin_family = AF_INET;
	this->addr.sin_port = htons(port);
	this->addr.sin_addr.s_addr = INADDR_ANY;
	
	appendpollfd(this->sfd);

	this->joinpass = pass;

	return (0);
}

// ! POTENTIAL CHANGES
// TODO remove addr such that its no longer needed in attributes
// TODO SEE IF caddr can be removed (pass NULL)

int	Server::accept_connect( void )
{
	if (poll(this->clientfds.data(), this->clientfds.size(), 10) == -1)
		throw FailedFunction("Poll");

	if (!(this->clientfds[0].revents & POLLIN))
		return (0);
	
	struct sockaddr_in	caddr;
	size_t addlen = sizeof(caddr);

	int cfd = accept(this->sfd, (struct sockaddr *) &caddr, (socklen_t *) &addlen);
	if (cfd == -1)
		throw FailedFunction("Accept");
	Client	login = Client(cfd, inet_ntoa(caddr.sin_addr));
	this->clients.insert(std::pair<int, Client>(cfd, login));
	std::cout << GREEN "New Connection! Socket " << cfd << RESET "\n";
	return (appendpollfd(cfd));
}

void	printBuffer(std::string buff)
{
	for (unsigned long i = 0; i < buff.size(); i++)
	{
		if (buff[i] == ' ')
			std::cout << "\n";
		else
			std::cout << (int) buff[i] << "   ";
	}
}

//TODO Figure out and handle receiving authentications from the client: "Irssi"
int Server::HandleClients()
{
	char buffer[BUFFER_SIZE];
	int	valread;
	
	for (size_t i = 1; i < clientfds.size(); i++)
	{
		if (this->clientfds[i].revents & POLLIN)
		{
			valread = recv(this->clientfds[i].fd, buffer, BUFFER_SIZE, 0);
			std::memset(buffer + valread, 0, BUFFER_SIZE - valread);
			if (valread < 0)
				throw FailedFunction("Recv");
			else if (valread == 0)
				removeUser(this->clientfds[i--].fd);
			else
			{
				// do {
				// 	this->clients[this->clientfds[i].fd].appendExecBuffer(buffer, this);
				// 	valread = recv(this->clientfds[i].fd, buffer, BUFFER_SIZE, 0);
				// 	std::memset(buffer + valread, 0, BUFFER_SIZE - valread);
				// } while (valread == BUFFER_SIZE);
				// ! NEED A LOOP (SUCH AS A FIXED VERSION OF ABOVE) to account for commands more than BUFFERSIZE
				if (valread)
					this->clients[this->clientfds[i].fd].appendExecBuffer(buffer, this);
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
		throw FailedFunction("Bind");
	
	if (listen(this->sfd, 3))
		throw FailedFunction("Listen");

	std::cout << GREEN "Server Started! Welcoming Clients!" RESET "\n";

	while (true)
		if (accept_connect() == -1 || HandleClients() == -1)
			return (1);

	return (0);
}
