/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:01:40 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/17 17:43:45 by mgoltay          ###   ########.fr       */
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
	// this->topic = f.topic;
	// this->password = f.password;
	// this->inviteonly = f.inviteonly;
	// this->trestrict = f.trestrict;
	// this->userlimit = f.userlimit;
	this->master = f.getMaster();
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

std::string	Channel::getPassword( void ) const
{
	return (this->password);
}

bool	Channel::isInviteOnly( void ) const 
{
	return (this->inviteonly);
}

Client		*Channel::getMaster( void ) const
{
	return (this->master);
}

int	Channel::getSize( void )
{
	return (users.size());
}

void	Channel::setMaster(Client &c)
{
	this->master = &c;
}

bool Channel::exists(std::map<int, Client *> map, Client c)
{
	std::map<int, Client *>::iterator it = map.find(c.getSocketFd());
	return (it != map.end());
}

void	Channel::broadcast(Client &c, std::string msg)
{
	std::string newmsg = GREEN + c.getNickname() + ": " + msg + RESET + "\n";

	std::map<int, Client *>::iterator it;
	for (it = users.begin(); it != users.end(); it++)
		(users[c.getSocketFd()])->sendmsg(newmsg);
}

void	Channel::kick(Client &kickee)
{
	if (!exists(this->users, kickee) || kickee.getSocketFd() == this->master->getSocketFd())
		return ;
	this->users.erase(kickee.getSocketFd());
	if (exists(this->ops, kickee))
		this->ops.erase(kickee.getSocketFd());
}

void	Channel::invite(Client &invitee)
{
	if (!exists(this->users, invitee))
		this->users.insert(std::pair<int, Client *>(invitee.getSocketFd(), &invitee));
	// * broadcast to all ops
}

void	Channel::setTopic(std::string t)
{
	this->topic = t;
}

void	Channel::mode(bool sign, char mode, std::string *parameters)
{
	if (mode == 'i')
		this->inviteonly = sign;
	else if (mode == 't')
		this->trestrict = sign;
	else if (mode == 'k' && !sign)
		this->password = "";
	else if (mode == 'k' && sign)
		this->password = *parameters; // ! needs fixing and broadcast to ops
}

// void	Channel::botfuncs();
