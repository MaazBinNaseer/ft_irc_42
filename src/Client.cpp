/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:24:58 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/14 19:05:26 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

t_cap genDefaultCap()
{
	t_cap caps;
	caps.echo_msg = false;
	caps.ext_join = false;
	caps.inv_notif = false;
	return (caps);
}

Client::Client()
{
	_cap_order = false;
	this->_caps = genDefaultCap();
}

Client::Client(unsigned int fd, std::string hostname): _socket_fd(fd), _hostname(hostname)
{
	_servername = "";
	_username = "";
	_nickname = "";
	_realname = "";
	_pass = false;
	_registered = false;
	_remove = false;
	_reason = "";
	_receiveBuffer = "";
	_cap_order = false;
	_factindex = 0;
	_triviaMode = false;
	_triviaindex = 0;
	this->_caps = genDefaultCap();
}

Client::Client( const Client &f )
{
	if (this != &f)
	{
		this->_socket_fd = f._socket_fd;
		this->_username = f._username;
		this->_hostname = f._hostname;
		this->_nickname = f._nickname;
		this->_pass = f._pass;
		this->_registered = f._registered;
		this->_remove = f._remove;
		this->_reason = f._reason;
		this->_receiveBuffer = f._receiveBuffer;
		this->_cap_order = f._cap_order;
		this->_factindex = f._factindex;
		this->_triviaMode = f._triviaMode;
		this->_triviaindex = f._triviaindex;
		this->_caps = f.getCaps();
	}
}

Client & Client::operator=(Client const & rhs)
{
	if (this != &rhs)
	{
		this->_socket_fd = rhs._socket_fd;
		this->_username = rhs._username;
		this->_hostname = rhs._hostname;
		this->_nickname = rhs._nickname;
		this->_pass = rhs._pass;
		this->_registered = rhs._registered;
		this->_remove = rhs._remove;
		this->_reason = rhs._reason;
		this->_receiveBuffer = rhs._receiveBuffer;
		this->_cap_order = rhs._cap_order;
		this->_factindex = rhs._factindex;
		this->_triviaMode = rhs._triviaMode;
		this->_triviaindex = rhs._triviaindex;
		this->_caps = rhs.getCaps();
	}
	return (*this);
}

Client::~Client() 
{
	_pass = false;
	_registered = false;
	_cap_order = false;
	this->_caps = genDefaultCap();
}

// *----- Attribute getters -----

int Client::getSocketFd() const
{
	return (this->_socket_fd);
}

std::string Client::getServername() const
{
	return (this->_servername);
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
	if (this->_nickname.empty())
		return ("*");
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

bool Client::getRemove() const
{
	return (this->_remove);
}

std::string Client::getReason() const
{
	return (this->_reason);
}

std::string &Client::getReceiveBuffer()
{
	return (this->_receiveBuffer);
}

std::deque<std::string>	&Client::getSendBuffer(void)
{
	return (this->_sendBuffer);
}

t_cap Client::getCaps(void) const
{
	return (this->_caps);
}

unsigned int Client::getFactIndex(void) const
{
	return (this->_factindex);
}

bool Client::getTriviaMode(void) const
{
	return (this->_triviaMode);
}

unsigned int Client::getTriviaIndex(void) const
{
	return (this->_triviaindex);
}

std::string Client::getTriviaAnswer(void) const
{
	return (this->_triviaAnswer);
}

// *----- Attribute setters -----

void Client::setSocketFd(int fd)
{
	this->_socket_fd = fd;
}

void Client::setServername(std::string servername)
{
	this->_servername = servername;
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

void Client::setRegistered(bool set)
{
	this->_registered = set;
}

void Client::setRemove(bool set)
{
	this->_remove = set;
}

void Client::setReason(std::string reason)
{
	if (reason.empty())
		this->_reason = "leaving";
	else
		this->_reason = reason;
}

void Client::pushSendBuffer(std::string string)
{
	this->_sendBuffer.push_back(string);
}

void Client::setCaps(unsigned long i, bool state)
{
	if (i == 0)
		this->_caps.echo_msg = state;
	else if (i == 1)
		this->_caps.ext_join = state;
	else if (i == 2)
		this->_caps.inv_notif = state;
}

void Client::setFactIndex(unsigned int index)
{
	if (index <= 15)
		this->_factindex = index;
	else
		this->_factindex = 0;
}

void Client::setTriviaMode(bool status)
{
	this->_triviaMode = status;
}

void Client::setTriviaIndex(unsigned int index)
{
	if (index <= 1)
		this->_triviaindex = index;
	else
		this->_triviaindex = 0;
}

void Client::setTriviaAnswer(std::string answer)
{
	this->_triviaAnswer = answer;
}

// *----- Other functions -----

void	Client::appendExecBuffer(std::string newbuff, Server *_serv)
{
	logRecv(newbuff, this->getSocketFd());
	this->_receiveBuffer += newbuff;
	while (this->_receiveBuffer.find('\n') != std::string::npos)
		Commands	extract(this, _serv, getReceiveBuffer());
}

void	Client::sendmsg(std::string msg)
{
	// * you can send time if client has server_time activated.
	if (send(getSocketFd(), msg.data(), msg.size(), 0) == -1)
		throw FailedFunction("Send");
}

void	Client::print(std::string color)
{
	std::cout << color << "SOCKET: \"" << getSocketFd() << "\" (" << getNickname() <<")\n" RESET;
}