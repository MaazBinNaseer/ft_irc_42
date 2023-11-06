/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFuncs.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:31:31 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/06 19:26:56 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

bool Server::countDown()
{
	if (this->counter > 0)
	{
		std::string append_c = intToString(counter);
		std::string message = RED "Closing down in ---" + append_c + RESET; 
		for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			Client *broad = getClientNick(it->second.getNickname());
			selfCommand(*broad, "EXIT", message);
		}
		usleep(1000000);
		this->counter--;
	}
	return (getClients().empty());
}

void	Server::clientDisconnect(std::string reason, int cfd)
{
	logDisconnect(reason, cfd);
	std::cout << RED "Client disconnected! Socket " << cfd << RESET "\n";
	this->removeUser(cfd);
}

void	Server::deliverToClient(Client &client)
{
	std::deque<std::string> &messages = client.getSendBuffer();
	std::string deliver;

	if (!messages.empty())
		logSend(messages, client.getSocketFd());
	while(!messages.empty())
	{
		deliver = messages.front();
		messages.pop_front();
		std::cout << "SENDING: " << deliver << std::endl;
		client.sendmsg(deliver);
	}
	if (client.getRemove() || (getShutdown() && this->counter == 0))
		clientDisconnect(client.getReason(), client.getSocketFd());
}

// ! POTENTIAL CHANGES
// TODO remove addr such that its no longer needed in attributes
// TODO SEE IF caddr can be removed (pass NULL)

int Server::HandleClients()
{
	char	buffer[BUFFER_SIZE];
	int		valread;
	
	for (size_t i = 1; i < clientfds.size(); i++)
	{
		pollfd currentClient = this->clientfds[i];
		Client *clientListSelect = &this->clients[currentClient.fd];
		if (currentClient.revents & POLLIN)
		{
			valread = recv(currentClient.fd, buffer, BUFFER_SIZE, 0);
			std::memset(buffer + valread, 0, BUFFER_SIZE - valread);
			if (valread < 0)
				throw FailedFunction("Recv");
			else if (valread == 0)
				clientListSelect->setReason("Used signal to leave");
			else
				clientListSelect->appendExecBuffer(buffer, this);
		}
		if (currentClient.revents & POLLHUP)
			clientDisconnect(clientListSelect->getReason(), clientListSelect->getSocketFd());
		else if (currentClient.revents & POLLOUT)
			this->deliverToClient(*clientListSelect);
	}
	return (0);
}

int	Server::appendpollfd(int new_socket)
{
	struct pollfd mypoll;

	std::memset(&mypoll, 0, sizeof(mypoll));
	mypoll.fd = new_socket;
	if (new_socket == 3)
		mypoll.events = POLLIN;
	else if (new_socket >= 3)
		mypoll.events = POLLIN | POLLOUT;
	mypoll.revents = 0;
	this->clientfds.push_back(mypoll);

	return (new_socket);
}

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
	std::cout << GREEN "Client Connected! Socket " << cfd << RESET "\n";
	logConnect(cfd);
	return (appendpollfd(cfd));
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

int	Server::bootup(char	*portstr, char *pass)
{
	logStart();
	if (assign(portstr, pass))
		return (1);

	if (bind(this->sfd, (struct sockaddr *) &this->addr, sizeof(this->addr)))
		throw FailedFunction("Bind");
	
	if (listen(this->sfd, 3))
		throw FailedFunction("Listen");

	std::cout << GREEN "Server Started! Welcoming Clients!" RESET "\n";
	while (true)
	{
		if (accept_connect() == -1 || HandleClients() == -1)
			return (1);
		if (getShutdown())
		{
			if (countDown())
				break ;
		}
	}
	return (0);
}
