/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:24:58 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/18 21:34:56 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

Client::Client()
{
	_cap_order = false;
}

Client::Client(unsigned int fd, std::string hostname): _socket_fd(fd), _client_id(fd), _hostname(hostname)
{
	_cap_order = false;
	this->_nickname = "AMMAazRuhan";
}

Client & Client::operator=(Client const & rhs)
{
	if (this != &rhs)
	{
		this->_socket_fd = rhs._socket_fd;
		this->_client_id = rhs._socket_fd;
		this->_username = rhs._username;
		this->_nickname = rhs._nickname;
	}
	return (*this);
}

Client::~Client() 
{

}

/*----- Attribute getters -----*/

int Client::getSocketFd()
{
	return (this->_socket_fd);
}

int Client::getClientId()
{
	return (this->_client_id);
}

std::string Client::getUsername()
{
	return (this->_username);
}

std::string Client::getHostname()
{
	return (this->_hostname);
}

std::string Client::getNickname()
{
	return (this->_nickname);
}

std::string Client::getRealname()
{
	return (this->_realname);
}

bool Client::getPass()
{
	return (this->_pass);
}

bool Client::getRegistered()
{
	return (this->_registered);
}

std::string &Client::getReceiveBuffer()
{
	return (this->_receiveBuffer);
}

std::vector<std::string>	&Client::getSendBuffer(void)
{
	return (this->_sendBuffer);
}

/*----- Attribute setters -----*/

void Client::setSocketFd(int fd)
{
	this->_socket_fd = fd;
}

void Client::setClientId(int fd)
{
	this->_client_id = fd;
}

void Client::setUsername(std::string username)
{
	this->_username = username;
}

void Client::setHostname(std::string hostname)
{
	this->_hostname = hostname;
}

void Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void Client::setRealname(std::string realname)
{
	this->_realname = realname;
}

void Client::setPass(bool set)
{
	this->_pass = set;
}

void Client:: setRegistered(bool set)
{
	this->_registered = set;
}

/*----- Other functions -----*/

void	Client::fillInfo(int fd, std::string username, std::string nickname)
{
	this->setSocketFd(fd);
	this->setClientId(fd);
	this->setUsername(username);
	this->setNickname(nickname);
}

void	Client::postInfo(void)
{
	std::cout << "Hello, my name is: " << YELLOW << this->_username << RESET << std::endl;
	std::cout << "with my hostname of: " << YELLOW << this->_hostname << RESET << std::endl;
	std::cout << "and my Nick name is: " << YELLOW << this->_nickname << RESET << std::endl;
	std::cout << "with a socket fd of: " << YELLOW << this->_socket_fd << RESET << std::endl;
	std::cout << "and a client id of: " << YELLOW << this->_client_id << RESET << std::endl;
}

void	Client::sendmsg(std::string msg)
{
	// * you can send time if client has server_time activated.
	send(getSocketFd(), msg.data(), msg.size(), 0);
}

void	Client::print(std::string color)
{
	std::cout << color << "SOCKET: \"" << getSocketFd() << "\" (" << getNickname() <<")\n" RESET;
}