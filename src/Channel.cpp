/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:01:40 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/17 18:56:01 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

// *----- Canonical Orthodox Form -----

Channel::Channel( void ) : name("Default")
{
	this->topic = "";
	this->password = "";
	this->inviteonly = false;
	this->trestrict = false;
	this->userlimit = -1;
}

Channel::Channel(std::string n, Client &c) : name(n)
{
	this->topic = "";
	this->password = "";
	this->users.insert(std::pair<int, Client *>(c.getSocketFd(), &c));
	this->ops.insert(std::pair<int, Client *>(c.getSocketFd(), &c));
	this->inviteonly = false;
	this->trestrict = false;
	this->userlimit = -1;

	std::cout << YELLOW "Channel " << n << " created!" RESET "\n";
}

Channel::Channel(std::string n, bool isBotchannel) :  name(n), isBotChannel(isBotchannel) 
{
	if(!isBotChannel)
		return ;
	this->topic = "This is a bot channel, enter PRIVMSG #bot <number> to call a specific bot";
	this->password = "";
	this->inviteonly = false;
	this->trestrict = false;
	this->userlimit = 100;
}

Channel::Channel( const Channel &f ) : name(f.getName())
{
	this->topic = f.getTopic();
	this->password = f.getPassword();
	this->users = f.getUsers();
	this->ops = f.getOps();
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

// *----- Attribute Getters -----

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

bool	Channel::isInviteOnly( void ) const 
{
	return (this->inviteonly);
}

bool	Channel::hasTopicRestrictions( void ) const
{
	return (this->trestrict);
}

int	Channel::getSize( void )
{
	return (users.size());
}

Client	*Channel::getClientNick(std::string nick)
{
	std::map<int, Client *>::iterator it;
	for (it = this->users.begin(); it != this->users.end(); it++)
		if (it->second->getNickname() == nick)
			return (it->second);
	return (NULL);
}

// *----- Attribute Setters -----

void	Channel::setTopic(Client *c, std::string t)
{
	this->topic = t;
	if (c)
		broadcast(*c, "TOPIC", t);
}

