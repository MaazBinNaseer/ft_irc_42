/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFuncs.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:40:19 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/19 20:58:17 by mgoltay          ###   ########.fr       */
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

void	Channel::kick(Client *c, Client &kickee)
{
	if (!exists(kickee) || kickee.getSocketFd() == this->master->getSocketFd())
		return ;
	this->users.erase(kickee.getSocketFd());
	if (this->ops.find(kickee.getSocketFd()) != this->ops.end())
		this->ops.erase(kickee.getSocketFd());
	if (c)
		broadcast(*c, "*kicked " + kickee.getNickname() + " out of the channel*");
	else
		broadcast(kickee, "*left the channel*");
}

void	Channel::invite(Client *c, Client &invitee)
{
	if (exists(invitee))
		return ;
	if (userlimit >= 0 && userlimit <= (int) users.size())
	{
		if (c)
			c->sendmsg(RED "Cannot invite '" + invitee.getNickname() + "' to a Full Channel!" RESET "\n");
		else
			invitee.sendmsg(RED "Channel is Full! Cannot Join!" RESET "\n");
	}
	else
	{
		this->users.insert(std::pair<int, Client *>(invitee.getSocketFd(), &invitee));
		if (c)
			broadcast(*c, "*invited " + invitee.getNickname() + " to the channel*");
		else
			broadcast(invitee, "*joined the channel*");
	}
}

void	Channel::handleO(Client *c, bool sign, std::string parameter)
{
	Client	*potop = getClientNick(parameter);

	if (parameter == "")
		c->sendmsg(RED "Input User to Edit Channel Operator Privilege!" RESET "\n");
	else if (!potop)
		c->sendmsg(RED "User does not exist in this Channel!" RESET "\n");
	else if (potop == master)
		c->sendmsg(RED "Trying to edit Master Operator Privileges!" RESET "\n");
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
			int	limit = atoi(parameter.c_str());
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

// · i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// · l: Set/remove the user limit to channel

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
