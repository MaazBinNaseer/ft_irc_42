/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:01:40 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/17 20:44:35 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

Channel::Channel(std::string n, Client &c) : name(n)
{
	this->topic = "[Set a topic]";
	this->password = "";
	this->users.insert(std::pair<int, Client *>(c.getSocketFd(), &c));
	this->ops.insert(std::pair<int, Client *>(c.getSocketFd(), &c));
	this->master = &c;
	this->inviteonly = false;
	this->trestrict = false;
	this->userlimit = -1;

	std::cout << YELLOW "Channel " << n << " created!" RESET "\n";
}

Channel::Channel( void ) : name("Default") // ! NEVER CALL DEFAULT
{
	this->topic = "[Set a topic]";
	this->password = "";
	this->master = NULL;
	this->inviteonly = false;
	this->trestrict = false;
	this->userlimit = -1;
}

Channel::Channel( const Channel &f ) : name(f.getName())
{
	this->topic = f.topic;
	this->password = f.getPassword();
	this->users = f.getUsers();
	this->ops = f.getOps();
	this->master = f.getMaster();
	this->inviteonly = false;
	this->trestrict = false;
	this->userlimit = -1;
}

Channel &Channel::operator=( const Channel &f )
{
	// this->topic = f.topic;
	// this->password = f.password;
	// this->inviteonly = f.inviteonly;
	// this->trestrict = f.trestrict;
	// this->userlimit = f.userlimit;
	(void) f;
	return (*this);
}

Channel::~Channel( void )
{

}

std::string	Channel::getName( void ) const
{
	return (this->name);
}

std::string	Channel::getTopic( void ) const
{
	return (this->topic);
}

std::string	Channel::getPassword( void ) const
{
	return (this->password);
}

std::map<int, Client *>	Channel::getUsers( void ) const
{
	return (this->users);
}

std::map<int, Client *>	Channel::getOps( void ) const
{
	return (this->ops);
}

Client	*Channel::getMaster( void ) const
{
	return (this->master);
}

bool	Channel::isInviteOnly( void ) const 
{
	return (this->inviteonly);
}

int	Channel::getSize( void )
{
	return (users.size());
}

void	Channel::setMaster(Client &c)
{
	this->master = &c;
}

void	Channel::setTopic(std::string t)
{
	this->topic = t;
}
