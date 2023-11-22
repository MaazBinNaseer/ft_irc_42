/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFuncs.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:40:19 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/22 13:45:55 by amalbrei         ###   ########.fr       */
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
	std::map<int, Client *>::iterator it;
	for (it = users.begin(); it != users.end(); it++)
		if (c.getSocketFd() != it->second->getSocketFd() || c.getCaps().echo_msg)
			broadcastallCommand(*it->second, c, cmd + S + this->getName(), msg);
}

void	Channel::broadcastOps(Client *c, std::string msg)
{
	std::string newmsg;

	newmsg = msg;
	// if (c)
	// 	newmsg = PURPLE "[" + getName() + "] " GREEN + c->getNickname() + ": " YELLOW + msg + RESET;
	// else
	// 	newmsg = PURPLE "[" + getName() + "] " YELLOW + msg + RESET;

	std::map<int, Client *>::iterator it;
	for (it = ops.begin(); it != ops.end(); it++)
		broadcastallCommand(*it->second, *c, "MODE" S + this->getName() + S + newmsg, ":");
}

int	Channel::kick(Client *c, Client &kickee, std::string command)
{
	if (!exists(kickee))
		return (0);
	this->users.erase(kickee.getSocketFd());
	if (this->ops.size() - 1 == 0 && this->getSize() != 0)
		handleO(c, true, this->users.begin()->second->getNickname(), "+");
	if (this->ops.find(kickee.getSocketFd()) != this->ops.end())
		this->ops.erase(kickee.getSocketFd());
	serverLog(kickee, this->getName(), "Has been removed to the target channel");
	
	std::string newmsg;
	if (c && c->getSocketFd() != kickee.getSocketFd())
	{
		newmsg = "*kicked " RED "'" + kickee.getNickname() + "'" YELLOW " out of the channel*";
		selfCommand(kickee, "", CYAN "*You have been kicked from " + this->getName() +  "*" RESET);
		for (std::map<int, Client *>::iterator it = users.begin(); it != users.end(); it++)
			broadcastTripleCommand(*it->second, *c, command + S + getName() + S + kickee.getNickname(), newmsg);
	}
	newmsg = PURPLE "*left the channel*" RESET;
	if (kickee.getCaps().ext_join)
		newmsg += " :" + kickee.getRealname();
	broadcast(kickee, "PART", newmsg);
	
	selfCommand(kickee, "PART" S + this->getName(), newmsg);
	return (this->getSize() == 0);
}

void	Channel::invite(Client *c, Client &invitee, std::string command)
{
	if (exists(invitee))
		return ;
	if (c && userlimit >= 0 && userlimit <= (int) users.size())
		throw CommandError("Channel Full", ERR_CHANNELISFULL, "Channel is Full! Cannot Invite!", *c);
	else if (!c && userlimit >= 0 && userlimit <= (int) users.size())
	{
		selfCommand(invitee, "471" S + invitee.getNickname() + S + this->getName(), "Channel is Full! Cannot Join!");
		throw CommandError("Channel Full", ERR_CHANNELISFULL, "Channel is Full! Cannot Join!", invitee);
	}
	this->users.insert(std::pair<int, Client *>(invitee.getSocketFd(), &invitee));
	serverLog(invitee, this->getName(), "Has been added to the target channel");
	
	std::string newmsg;
	if (c)
	{
		newmsg = YELLOW "*invited " + invitee.getNickname() + " to the channel*" + RESET;
		for (std::map<int, Client *>::iterator it = users.begin(); it != users.end(); it++)
			if (it->second->getCaps().inv_notif)
				broadcastTripleCommand(*it->second, *c, command + S + invitee.getNickname() + S + getName(), newmsg);
	}
	newmsg = YELLOW "*joined the channel*" RESET;
	if (invitee.getCaps().ext_join)
		newmsg += " :" + invitee.getRealname();
	broadcast(invitee, "JOIN", newmsg);

	selfCommand(invitee, "JOIN" S + this->getName(), newmsg);
	messageCommand(invitee, this->getName(), "PRIVMSG", GREEN "Welcome to \"" + this->getName() + "\", topic of the channel: " YELLOW + this->topic + RESET);
	selfCommand(invitee, "332" S + invitee.getNickname() + S + this->getName() , this->getTopic());
}

void	Channel::handleO(Client *c, bool sign, std::string parameter, std::string modes)
{
	Client	*potop = getClientNick(parameter);

	if (parameter == "")
		selfCommand(*c, "PRIVMSG" S + this->getName(), RED "Input User to Edit Channel Operator Privilege!" RESET);
	else if (!potop)
		selfCommand(*c, "PRIVMSG" S + this->getName(), RED "User does not exist in this Channel!" RESET);
	else
	{
		modes += "o" S + parameter; 
		if (sign == true && !isOp(*potop))
		{
			this->ops.insert(std::pair<int, Client *>(potop->getSocketFd(), potop));
			targettedCommand(*c, *potop, "PRIVMSG" S + this->getName(), "*Gave you operator privileges*");
			broadcastOps(c, modes);
		}
		else if (sign == false && isOp(*potop))
		{
			broadcastOps(c, modes);
			this->ops.erase(potop->getSocketFd());
			targettedCommand(*c, *potop, "PRIVMSG" S + this->getName(), "*Took you operator privileges*");
		}
	}
}

void	Channel::handleL(Client *c, bool sign, std::string parameter, std::string modes)
{
	if (!sign)
		this->userlimit = -1;
	else if (parameter == "")
		selfCommand(*c, "PRIVMSG" S + this->getName(), RED "Input User Limit!" RESET);
	else
	{
		try {
			int	limit = atoi(parameter.c_str());
			if (limit < (int) this->users.size())
				selfCommand(*c, "PRIVMSG" S + this->getName(), RED "Channel Size is Greater than Suggested Limited!" RESET);
			else
			{
				this->userlimit = limit;
				modes += "l " + intToString(limit);
				broadcastOps(c, modes);
			}
		} catch(std::exception &e) {
			(void) e;
			selfCommand(*c, "PRIVMSG" S + this->getName(), RED "Send an Appropriate Number!" RESET);
		}
	}
}

void	Channel::mode(Client *c, bool sign, char mode, std::string parameter)
{
	std::string modes = "-";
	if (sign)
		modes = "+";
	if (mode == 'i')
	{
		this->inviteonly = sign;
		modes += "i";
		broadcastOps(c, modes);
	}
	else if (mode == 't')
	{
		this->trestrict = sign;
		modes += "t";
		// std::string mes = "channel is";
		// if (!sign)
		// 	mes += " no longer ";
		// mes += "topic-restricted";
		broadcastOps(c, modes);
	}
	else if (mode == 'k' && !sign)
	{
		this->password = "";
		modes += "k";
		broadcastOps(c, modes);
	}
	else if (mode == 'k' && parameter != "")
	{
		this->password = parameter;
		modes += "k " + parameter;
		broadcastOps(c, modes);
	}
	else if (mode == 'o')
		handleO(c, sign, parameter, modes);
	else if (mode == 'l')
		handleL(c, sign, parameter, modes);		
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
