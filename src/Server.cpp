/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:16:49 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/26 13:12:56 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

Server::Server( void )
{
	std::ofstream log("Serverlog.txt");
	log.close();
	logStart();
	this->sfd = -1;
	this->operpass = OPERPASS;
	this->counter  = 3;
	createBotChannel();
	// std::cout << YELLOW "Default Server constructor called" RESET "\n";
}

Server::Server( const Server &f )
{
	// std::cout << YELLOW "Copy Server constructor called" RESET "\n";
	this->operpass = f.getOperPass();
}

Server &Server::operator=( const Server &f )
{
	// std::cout << YELLOW "Copy Server assignment operator called" RESET "\n";
	this->operpass = f.getOperPass();
	return (*this);
}

Server::~Server( void )
{
	logEnd();
	if (this->sfd != -1)
		close(this->sfd);
	// std::cout << YELLOW "Server Destructor called" RESET "\n";
}

int	Server::getFdSize(void)
{
	return (this->clientfds.size());
}

int Server::getCounter(void)
{
	return (this->counter);
}

std::string	Server::getPassword(void) const
{
	return (this->joinpass);
}

std::string	Server::getOperPass(void) const
{
	return (this->operpass);
}

Client	*Server::getClient(int cfd)
{
	if (this->clients.find(cfd) != this->clients.end())
		return (&this->clients[cfd]);
	return (NULL);
}

Client	*Server::getClientNick(std::string nick)
{
	std::map<int, Client>::iterator it;
	for (it = this->clients.begin(); it != this->clients.end(); it++)
		if (it->second.getNickname() == nick)
			return (&it->second);
	return (NULL);
}

Client	*Server::getClientFd(int fd)
{
	std::map<int, Client>::iterator it;
	for (it = this->clients.begin(); it != this->clients.end(); it++)
		if (it->second.getSocketFd() == fd)
			return (&it->second);
	return (NULL);
}

Client	*Server::getClientUser(std::string user)
{
	std::map<int, Client>::iterator it;
	for (it = this->clients.begin(); it != this->clients.end(); it++)
		if (it->second.getUsername() == user)
			return (&it->second);
	return (NULL);
}

std::map<int, Client>	&Server::getClients(void)
{
	return (this->clients);
}

Channel	*Server::getChannel(std::string name)
{
	if (this->channels.find(name) != this->channels.end())
		return (&this->channels[name]);
	if (name[0] == '#')
		return (getChannel(name.substr(1)));
	return (NULL);
}

std::map<std::string, Channel*>	Server::getChannels(Client *c)
{
	std::map<std::string, Channel*> chansin;

	for (std::map<std::string, Channel>::iterator it= this->channels.begin(); it != this->channels.end(); it++)
			if (it->second.exists(*c))
				chansin[it->first] = &it->second;
	return (chansin);
}

std::map<std::string, Channel>	&Server::getChannels()
{
	return (this->channels);
}

Bot& Server::getBot()
{
	return (botInstance);
}

bool	Server::isUser(Client &c)
{
	return (this->clients.find(c.getSocketFd()) != this->clients.end());
}

bool	Server::isOp(Client &c)
{
	return (this->operators.find(c.getSocketFd()) != this->operators.end());
}

void	Server::setPassword(std::string pass)
{
	this->joinpass = pass;
}

void	Server::removeUser(int cfd)
{
	if (!getClient(cfd))
		return ;
	for (std::map<std::string, Channel>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
		if (it->second.exists(*getClient(cfd)))
			it->second.kick(NULL, *getClient(cfd), "");
	for (size_t i = 0; i < this->clientfds.size(); i++)
		if (this->clientfds[i].fd == cfd)
			this->clientfds.erase(this->clientfds.begin() + i--);
	if (isOp(*getClient(cfd)))
		this->operators.erase(cfd);
	serverLog(this->clients[cfd], "", "Has been removed from the server");
	this->clients.erase(cfd);
	close(cfd);
}

void	Server::setOperPass(std::string pass)
{
	this->operpass = pass;
}

void	Server::addOperator(Client *potop)
{
	this->operators.insert(std::pair<int, Client *>(potop->getSocketFd(), potop));
}

void	Server::addChannel(std::string name, Client &c)
{
	if (getChannel(name) || getClientNick(name))
		return ;
	Channel ch = Channel(name, c);
	this->channels.insert(std::pair<std::string, Channel>(name, ch));
}

void	Server::removeChannel(std::string name)
{
	std::cout << YELLOW "Channel " + name + " removed!\n" + RESET; 
	this->channels.erase(name);
}

void	Server::decrementCounter()
{
	this->counter--;
}

void Server::createBotChannel()
{
	if(this->channels.find("#bot") == this->channels.end())
	{
		Channel bot = Channel("#bot", true);
		this->channels.insert(std::pair<std::string, Channel>("#bot", bot));
	}
}
