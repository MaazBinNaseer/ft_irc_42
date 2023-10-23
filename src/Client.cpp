/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:24:58 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/23 17:59:14 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

t_cap genDefaultCap()
{
	t_cap caps;
	caps.echo_msg = true;
	caps.ext_join = false;
	caps.inv_notif = true;
	return (caps);
}

Client::Client()
{
	_cap_order = false;
	this->_caps = genDefaultCap();
}

Client::Client(unsigned int fd, std::string hostname): _socket_fd(fd), _client_id(fd), _hostname(hostname)
{
	this->_nickname = "AMMAazRuhan";
	_cap_order = false;
	this->_caps = genDefaultCap();
}

Client::Client( const Client &f )
{
	if (this != &f)
	{
		this->_socket_fd = f._socket_fd;
		this->_client_id = f._socket_fd;
		this->_username = f._username;
		this->_nickname = f._nickname;
		this->_caps = f.getCaps();
	}
}

Client & Client::operator=(Client const & rhs)
{
	if (this != &rhs)
	{
		this->_socket_fd = rhs._socket_fd;
		this->_client_id = rhs._socket_fd;
		this->_username = rhs._username;
		this->_nickname = rhs._nickname;
		this->_caps = rhs.getCaps();
	}
	return (*this);
}

Client::~Client() 
{

}

/*----- Attribute getters -----*/

int Client::getSocketFd() const
{
	return (this->_socket_fd);
}

int Client::getClientId() const
{
	return (this->_client_id);
}

std::string Client::getUsername() const
{
	return (this->_username);
}

std::string Client::getHostname() const
{
	return (this->_hostname);
}

std::string Client::getNickname() const
{
	return (this->_nickname);
}

std::string Client::getRealname() const
{
	return (this->_realname);
}

bool Client::getPass() const
{
	return (this->_pass);
}

bool Client::getRegistered() const
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

t_cap	Client::getCaps(void) const
{
	return (this->_caps);
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
	send(getSocketFd(), msg.data(), msg.size(), 0);
}

void	Client::appendExecBuffer(std::string newbuff, Server *_serv)
{
	this->_receiveBuffer += newbuff;
	while (this->_receiveBuffer.find('\n') != std::string::npos)
		Commands	extract(this, _serv, getReceiveBuffer());
}

void	Client::print(std::string color)
{
	std::cout << color << "SOCKET: \"" << getSocketFd() << "\" (" << getNickname() <<")\n" RESET;
}