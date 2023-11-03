/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:01:40 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/21 14:16:59 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

Channel::Channel(std::string n, Client &c, bool isBotChannel) : name(n), isBotChannel(false)
{	
	if(isBotChannel == true)
	{
		this->topic ="Welcome to the bot channel";
		this->password = "";
	}
	else
		{
			this->topic = "[Set a topic]";
			this->password = "";
			this->users.insert(std::pair<int, Client *>(c.getSocketFd(), &c));
			this->ops.insert(std::pair<int, Client *>(c.getSocketFd(), &c));
			this->inviteonly = false;
			this->trestrict = false;
			this->userlimit = -1;
		}
	std::cout << YELLOW "Channel " << n << " created!" RESET "\n";
}
Channel::Channel(std::string n, bool isBotchannel) :  name(n), isBotChannel(isBotchannel) 
{
    // Set default properties for the bot channel
    this->topic = "Welcome to the bot channel!";
   	this->password = "";  // Let's assume it's a public channel without a password
    this->inviteonly = false;
    this->trestrict = false; // or true depending on your requirements
    this->userlimit = 100;
}


Channel::Channel( void ) : name("Default") // ! NEVER CALL DEFAULT
{
	this->topic = "[Set a topic]";
	this->password = "";
	this->inviteonly = false;
	this->trestrict = false;
	this->userlimit = -1;
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

void	Channel::setTopic(Client *c, std::string t)
{
	this->topic = t;
	if (c)
		broadcast(*c, " *changed the topic*");
}

Client	*Channel::getClientNick(std::string nick)
{
	std::map<int, Client *>::iterator it;
	for (it = this->users.begin(); it != this->users.end(); it++)
		if (it->second->getNickname() == nick)
			return (it->second);
	return (NULL);
}