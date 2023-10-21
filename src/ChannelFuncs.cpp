/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFuncs.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:40:19 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/21 14:20:25 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

bool Channel::exists(Client c)
{
	return (this->users.find(c.getSocketFd()) != this->users.end());
}

bool Channel::isOp(Client c)
{
	return (this->ops.find(c.getSocketFd()) != this->ops.end());
}

void	Channel::broadcast(Client &c, std::string msg)
{
	std::string newmsg = PURPLE "[" + getName() + "] " GREEN + c.getNickname() + ": " YELLOW + msg + RESET "\n";

	std::map<int, Client *>::iterator it;
	for (it = users.begin(); it != users.end(); it++)
		if (c.getSocketFd() != it->second->getSocketFd() || c.getCaps().echo_msg)
			it->second->sendmsg(newmsg);
}

void	Channel::broadcastOps(Client *c, std::string msg)
{
	std::string newmsg;
	if (c)
		newmsg = PURPLE "[" + getName() + "] " GREEN + c->getNickname() + ": " YELLOW + msg + RESET "\n";
	else
		newmsg = PURPLE "[" + getName() + "] " YELLOW + msg + RESET "\n";

	std::map<int, Client *>::iterator it;
	for (it = ops.begin(); it != ops.end(); it++)
		it->second->sendmsg(newmsg);
}

int	Channel::kick(Client *c, Client &kickee)
{
	if (!exists(kickee))
		return (0);
	this->users.erase(kickee.getSocketFd());
	if (this->ops.find(kickee.getSocketFd()) != this->ops.end())
		this->ops.erase(kickee.getSocketFd());
	if (c && c->getSocketFd() != kickee.getSocketFd())
		broadcast(*c, "*kicked " RED "'" + kickee.getNickname() + "'" YELLOW " out of the channel*");
	else
		broadcast(kickee, "*left the channel*");
	kickee.sendmsg(PURPLE "You are no longer part of channel '" + getName() + "'!" RESET "\n");
	if (this->ops.size() == 0 && getSize() != 0)
		handleO(c, true, this->users.begin()->second->getNickname());
	return (getSize() == 0);
}

void	Channel::invite(Client *c, Client &invitee)
{
	if (exists(invitee))
		return ;
	if (c && userlimit >= 0 && userlimit <= (int) users.size())
		c->sendmsg(RED "Cannot invite '" + invitee.getNickname() + "' to a Full Channel!" RESET "\n");
	else if (!c && userlimit >= 0 && userlimit <= (int) users.size())
		invitee.sendmsg(RED "Channel is Full! Cannot Join!" RESET "\n");
	else
	{
		this->users.insert(std::pair<int, Client *>(invitee.getSocketFd(), &invitee));
		
		std::string newmsg;
		if (c)
			newmsg = PURPLE "[" + getName() + "] " GREEN + c->getNickname() + ": " YELLOW + "*invited " + invitee.getNickname() + " to the channel*" + RESET "\n";
		else 
			newmsg = PURPLE "[" + getName() + "] " GREEN + invitee.getNickname() + ": " YELLOW + "*joined the channel*" + RESET "\n";
	
		for (std::map<int, Client *>::iterator it = users.begin(); it != users.end(); it++)
			if (it->second->getCaps().inv_notif)
				it->second->sendmsg(newmsg);
	}
}

void	Channel::handleO(Client *c, bool sign, std::string parameter)
{
	Client	*potop = getClientNick(parameter);

	if (parameter == "")
		c->sendmsg(RED "Input User to Edit Channel Operator Privilege!" RESET "\n");
	else if (!potop)
		c->sendmsg(RED "User does not exist in this Channel!" RESET "\n");
	else
	{
		if (sign == true && !isOp(*potop))
		{
			this->ops.insert(std::pair<int, Client *>(potop->getSocketFd(), potop));
			broadcastOps(c, "*made '" + parameter + "' a channel operator*");
		}
		else if (sign == false && isOp(*potop))
		{
			this->ops.erase(potop->getSocketFd());
			broadcastOps(c, "*removed '" + parameter + "'s operator privileges*");
		}
	}
}

void	Channel::handleL(Client *c, bool sign, std::string parameter)
{
	if (!sign)
		this->userlimit = -1;
	else if (parameter == "")
		c->sendmsg(RED "Input User Limit!" RESET "\n");
	else
	{
		try {
			int	limit = atoi(parameter.c_str()); // TODO CHECK IF STRING VALID FIRST
			if (limit < (int) this->users.size())
				c->sendmsg(RED "Channel Size is Greater than Suggested Limited!" RESET "\n");
			else
				this->userlimit = limit;
		} catch(std::exception &e) {
			(void) e;
			c->sendmsg(RED "Send an Appropriate Number!" RESET "\n");
		}
	}
}

void	Channel::mode(Client *c, bool sign, char mode, std::string parameter)
{
	if (mode == 'i')
		this->inviteonly = sign;
	else if (mode == 't')
		this->trestrict = sign;
	else if (mode == 'k' && !sign)
	{
		this->password = "";
		broadcastOps(c, "*has removed the password*");
	}
	else if (mode == 'k' && parameter != "")
	{
		this->password = parameter;
		broadcastOps(c, "*has changed the password*");
	}
	else if (mode == 'o')
		handleO(c, sign, parameter);
	else if (mode == 'l')
		handleL(c, sign, parameter);		
}

void	Channel::print(void)
{
	std::cout << CYAN "CHANNEL \"" << getName() << "\"\n" RESET;
	std::map<int, Client *>::iterator it;
	for (it = this->users.begin(); it != this->users.end(); it++)
		it->second->print(PURPLE);
	for (it = this->ops.begin(); it != this->ops.end(); it++)
		it->second->print(RED);
	std::cout << CYAN "SIZE \"" << getSize() << "\"\n" RESET "\n";
}

// void	Channel::botfuncs();
