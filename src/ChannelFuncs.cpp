/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFuncs.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:40:19 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/17 18:13:44 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

bool Channel::exists(Client &c)
{
	return (this->users.find(c.getSocketFd()) != this->users.end());
}

bool Channel::isOp(Client c)
{
	return (this->ops.find(c.getSocketFd()) != this->ops.end());
}

void	Channel::broadcast(Client &c, std::string cmd, std::string msg)
{
	std::string newmsg =  this->getName() + " " GREEN + msg + RESET;

	std::map<int, Client *>::iterator it;
	for (it = users.begin(); it != users.end(); it++)
		if (c.getSocketFd() != it->second->getSocketFd() || c.getCaps().echo_msg)
			broadcastallCommand(*it->second, c, cmd, newmsg);
}

void	Channel::broadcastOps(Client *c, std::string msg)
{
	std::string newmsg;
	if (c)
		newmsg = PURPLE "[" + getName() + "] " GREEN + c->getNickname() + ": " YELLOW + msg + RESET;
	else
		newmsg = PURPLE "[" + getName() + "] " YELLOW + msg + RESET;

	std::map<int, Client *>::iterator it;
	for (it = ops.begin(); it != ops.end(); it++)
		selfCommand(*it->second, "", newmsg);
}

int	Channel::kick(Client *c, Client &kickee)
{
	if (!exists(kickee))
		return (0);
	this->users.erase(kickee.getSocketFd());
	if (this->ops.find(kickee.getSocketFd()) != this->ops.end())
		this->ops.erase(kickee.getSocketFd());
	if (c && c->getSocketFd() != kickee.getSocketFd())
		broadcast(*c, "KICK/PART", "*kicked " RED "'" + kickee.getNickname() + "'" YELLOW " out of the channel*");
	else
		broadcast(kickee, "KICK/PART", "*left the channel*");
	selfCommand(kickee, "KICK/PART", PURPLE "You are no longer part of channel '" + getName() + "'!" RESET);
	serverLog(kickee, this->getName(), "Has left the target channel");
	if (this->ops.size() == 0 && getSize() != 0)
		handleO(c, true, this->users.begin()->second->getNickname());
	return (getSize() == 0);
}

void	Channel::invite(Client *c, Client &invitee, std::string command)
{
	if (exists(invitee))
		return ;
	if (c && userlimit >= 0 && userlimit <= (int) users.size())
		throw CommandError("Channel Full", ERR_CHANNELISFULL, "Channel is Full! Cannot Invite!", *c);
	else if (!c && userlimit >= 0 && userlimit <= (int) users.size())
		throw CommandError("Channel Full", ERR_CHANNELISFULL, "Channel is Full! Cannot Join!", invitee);
	else
	{
		this->users.insert(std::pair<int, Client *>(invitee.getSocketFd(), &invitee));
		serverLog(invitee, this->getName(), "Has been added to the target channel");
		
		std::string newmsg;
		if (c)
			newmsg = getName() + " :" YELLOW + "*invited " + invitee.getNickname() + " to the channel*" + RESET;
		else
		{
			newmsg = getName() + " :" YELLOW + "*joined the channel*" + RESET;
			if (invitee.getCaps().ext_join)
				newmsg += " :" + invitee.getRealname();
		}
	
		messageCommand(invitee, this->getName(), "TOPIC", GREEN "Welcome to \"" + this->getName() + "\", topic of the channel: " YELLOW + this->topic + RESET);
		for (std::map<int, Client *>::iterator it = users.begin(); it != users.end(); it++)
			if (it->second->getCaps().inv_notif)
				broadcastallCommand(*it->second, invitee, command, newmsg);
	}
}

void	Channel::handleO(Client *c, bool sign, std::string parameter)
{
	Client	*potop = getClientNick(parameter);

	if (parameter == "")
		selfCommand(*c, "", RED "Input User to Edit Channel Operator Privilege!" RESET);
	else if (!potop)
		selfCommand(*c, "", RED "User does not exist in this Channel!" RESET);
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
		selfCommand(*c, "", RED "Input User Limit!" RESET);
	else
	{
		try {
			int	limit = atoi(parameter.c_str());
			if (limit < (int) this->users.size())
				selfCommand(*c, "", RED "Channel Size is Greater than Suggested Limited!" RESET);
			else
				this->userlimit = limit;
		} catch(std::exception &e) {
			(void) e;
			selfCommand(*c, "", RED "Send an Appropriate Number!" RESET);
		}
	}
}

void	Channel::mode(Client *c, bool sign, char mode, std::string parameter)
{
	if (mode == 'i')
	{
		this->inviteonly = sign;
		std::string mes = "channel is";
		if (!sign)
			mes + " no longer ";
		mes + "invite-only";
		broadcastOps(c, mes);
	}
	else if (mode == 't')
	{
		this->trestrict = sign;
		std::string mes = "channel is";
		if (!sign)
			mes + " no longer ";
		mes + "topic-restricted";
		broadcastOps(c, mes);
	}
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
