/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerFuncs.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:31:31 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/24 16:49:46 by amalbrei         ###   ########.fr       */
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
			selfCommand(it->second, "EXIT", message);
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
		if (DEBUG == 2 || DEBUG == 3)
			std::cout << YELLOW "SENDING (to " + client.getNickname() + "): " RESET << deliver << "\n";
		client.sendmsg(deliver);
	}
	if (client.getRemove() || (this->getShutdown() && this->counter == 0))
		clientDisconnect(client.getReason(), client.getSocketFd());
}

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
			{
				clientListSelect->setRemove(true);
				clientListSelect->setReason("Used signal to leave");
			}
			else
				clientListSelect->appendExecBuffer(buffer, this);
		}
		if (currentClient.revents & POLLHUP)
			this->clientDisconnect(clientListSelect->getReason(), clientListSelect->getSocketFd());
		else if (currentClient.revents & POLLOUT)
			this->deliverToClient(*clientListSelect);
	}
	return (0);
}

int	Server::appendpollfd(int new_socket, bool is_server)
{
	struct pollfd mypoll;

	std::memset(&mypoll, 0, sizeof(mypoll));
	mypoll.fd = new_socket;
	if (is_server)
		mypoll.events = POLLIN;
	else if (!is_server)
		mypoll.events = POLLIN | POLLOUT;
	mypoll.revents = 0;
	this->clientfds.push_back(mypoll);

	return (new_socket);
}

int	Server::accept_connect( void )
{
	if (poll(this->clientfds.data(), this->clientfds.size(), 10) == -1)
		throw SignalTrigger();

	if (!(this->clientfds[0].revents & POLLIN))
		return (0);
	
	struct sockaddr_in	new_client_addr;
	socklen_t			new_client_addr_size;

	new_client_addr_size = sizeof(new_client_addr);

	int cfd = accept(this->sfd, (struct sockaddr *)&new_client_addr,
		&new_client_addr_size);
	if (cfd == -1)
		throw FailedFunction("Accept");
	
	Client	login = Client(cfd, inet_ntoa(new_client_addr.sin_addr));
	this->clients.insert(std::pair<int, Client>(cfd, login));
	std::cout << GREEN "Client Connected! Socket " << cfd << RESET "\n";

	logConnect(cfd);
	return (appendpollfd(cfd, false));
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
	if (this->sfd == -1 || fcntl(this->sfd, F_SETFL, O_NONBLOCK) < 0)
		throw FailedFunction("Socket");

	struct sockaddr_in	addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	appendpollfd(this->sfd, true);
	this->joinpass = pass;

	if (bind(this->sfd, (struct sockaddr *) &addr, sizeof(addr)))
		throw FailedFunction("Bind");

	return (0);
}

void	signalhandler(int signal)
{
	if (signal)
		std::cout << "\r" YELLOW "Clients Must Leave!   \b\b" RESET << std::flush;
}

int	Server::bootup(char	*portstr, char *pass)
{
	if (assign(portstr, pass))
		return (1);
	
	if (listen(this->sfd, 50))
		throw FailedFunction("Listen");
	
	std::cout << GREEN "Server Started! Welcoming Clients!" RESET "\n";
	signal(SIGINT,  signalhandler);
	signal(SIGQUIT, signalhandler);
	signal(SIGTSTP, signalhandler);
	while (true)
		if (accept_connect() == -1 || HandleClients() == -1 || (getShutdown() && countDown()))
			return (1 - getShutdown());
	return (0);
}

void	Server::print()
{
	std::cout << "Printing std::vector<pollfd>:\n";
    for (std::vector<pollfd>::const_iterator it = clientfds.begin(); it != clientfds.end(); ++it)
	{
        const pollfd& item = *it;
        std::cout << "fd: " << item.fd << ", events: " << item.events << ", revents: " << item.revents << "\n";
    }

    std::cout << "Printing std::map<int, Client>:\n";
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) 
		it->second.print(CYAN);
}
