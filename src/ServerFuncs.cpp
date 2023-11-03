/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFuncs.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:31:31 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/03 15:47:03 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

void Server::countDown()
{
	if (this->counter > 0)
	{
		std::string append_c = intToString(counter);
		std::string message = RED "Closing down in ---" + append_c + RESET "\r\n"; 
		for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
		{
			Client *broad = getClientNick(it->second.getNickname());
			selfCommand(*broad, "EXIT", message);
		}
		usleep(1000000);
			this->counter--;
	}
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
		// std::cout << "SENDING: " << deliver << std::endl;
		client.sendmsg(deliver);
	}
	if (client.getRemove() || (getShutdown() && this->counter == 0))
	{
		logDisconnect(client.getReason(), client.getSocketFd());
		std::cout << RED "Client disconnected! Socket " << client.getSocketFd() << RESET "\n";
		this->removeUser(client.getSocketFd());
	}
}

int Server::HandleClients()
{
	char	buffer[BUFFER_SIZE];
	int		valread;
	
	for (size_t i = 1; i < clientfds.size(); i++)
	{
		if (this->clientfds[i].revents & POLLIN)
		{
			valread = recv(this->clientfds[i].fd, buffer, BUFFER_SIZE, 0);
			std::memset(buffer + valread, 0, BUFFER_SIZE - valread);
			if (valread < 0)
				throw FailedFunction("Recv");
			else if (valread == 0)
			{
				this->clients[this->clientfds[i].fd].setRemove(true);
				this->clients[this->clientfds[i].fd].setReason("Used signal to leave");
			}
			else
				this->clients[this->clientfds[i].fd].appendExecBuffer(buffer, this);
		}
		if (this->clientfds[i].revents & POLLOUT)
			this->deliverToClient(this->clients[this->clientfds[i].fd]);
	}
	return (0);
}

int	Server::accept_connect( void )
{
	if (poll(this->clientfds.data(), this->clientfds.size(), 10) == -1)
		throw FailedFunction("Poll");

	if (!(this->clientfds[0].revents & POLLIN))
		return (0);

	int cfd = accept(this->sfd, NULL, NULL);
	if (cfd == -1)
		throw FailedFunction("Accept");

	Client	login = Client(cfd);
	this->clients.insert(std::pair<int, Client>(cfd, login));
	std::cout << GREEN "Client Connected! Socket " << cfd << RESET "\n";

	logConnect(cfd);
	return (appendpollfd(cfd));
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

	struct sockaddr_in	addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	appendpollfd(this->sfd);
	this->joinpass = pass;

	if (bind(this->sfd, (struct sockaddr *) &addr, sizeof(addr)))
		throw FailedFunction("Bind");

	return (0);
}

int	Server::bootup(char	*portstr, char *pass)
{
	logStart();
	if (assign(portstr, pass))
		return (1);
	
	if (listen(this->sfd, 3))
		throw FailedFunction("Listen");

	std::cout << GREEN "Server Started! Welcoming Clients!" RESET "\n";
	while (true)
	{
		if (accept_connect() == -1 || HandleClients() == -1)
			return (1);
		if (getShutdown())
		{
			countDown();
			if (getClients().empty())
			{
				close(this->sfd);
				break ;
			}
		}
	}
	return (0);
}
