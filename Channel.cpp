/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 19:01:40 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/15 21:14:25 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/ft_irc.hpp"

Channel::Channel(std::string n, Client &c) : name(n)
{
	this->topic = "[Set a topic]";
	this->password = "";
	this->users[c.getSocketFd()] = c;
	this->ops[c.getSocketFd()] = c;
	this->master = &c;
	this->inviteonly = false;
	this->trestrict = false;
	this->userlimit = -1;
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

void	sendmsg(Client c, std::string msg)
{
	(void) c; // TODO IMPLEMENT
	(void) msg;

	// * you can send time if client has server_time activated.
}

int	Channel::getSize( void )
{
	return (users.size());
}

void	Channel::setMaster(Client &c)
{
	this->master = &c;
}

bool Channel::exists(std::map<int, Client &> map, Client c)
{
	std::map<int, Client &>::iterator it = map.find(c.getSocketFd());
	return (it != map.end());
}

void	Channel::kick(Client &kickee)
{
	if (!exists(this->users, kickee))
		return ;
	this->users.erase(kickee.getSocketFd());
	if (exists(this->ops, kickee) && kickee.getSocketFd() != this->master->getSocketFd())
		this->ops.erase(kickee.getSocketFd());
}

void	Channel::invite(Client &invitee)
{
	if (!exists(this->users, invitee))
		this->users[invitee.getSocketFd()] = invitee;
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

void	join(Client	&potential, std::string name, std::string pass, std::map<std::string, Channel> & channels)
{
	if (channels.find(name) != channels.end())
	{
		if (channels[name].isInviteOnly())
			sendmsg(potential, RED "Channel is Invite Only!" RESET "\n");
		else if (channels[name].getPassword() != pass)
			sendmsg(potential, RED "Wrong Channel Password!" RESET "\n");
		else
			channels[name].invite(potential);
	}
	else
		channels[name] = Channel(name, potential);
	
}
