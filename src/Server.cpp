/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:16:49 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/18 21:43:11 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

Server::Server( void )
{
	// std::cout << YELLOW "Default Server constructor called" RESET "\n";
}

Server::Server( const Server &f )
{
	// std::cout << YELLOW "Copy Server constructor called" RESET "\n";
	(void)f;
}

Server &Server::operator=( const Server &f )
{
	// std::cout << YELLOW "Copy Server assignment operator called" RESET "\n";
	(void) f;
	return (*this);
}

Server::~Server( void )
{
	// std::cout << YELLOW "Server Destructor called" RESET "\n";
}

int	Server::getFd(void)
{
	return(this->sfd);
}

std::string	Server::getPassword(void)
{
	return (this->password);
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
	return (NULL);
}

std::map<std::string, Channel>	&Server::getChannels()
{
	return (this->channels);
}

void	Server::setPassword(std::string pass)
{
	this->password = pass;
}

void	Server::addChannel(std::string name, Client &c)
{
	if (getChannel(name) || getClientNick(name))
		return ;
	Channel ch = Channel(name, c);
	this->channels.insert(std::pair<std::string, Channel>(name, ch));
}

void	Server::print(void)
{
	std::map<std::string, Channel>::iterator it;
	for (it = this->channels.begin(); it != this->channels.end(); it++)
		it->second.print();
}
