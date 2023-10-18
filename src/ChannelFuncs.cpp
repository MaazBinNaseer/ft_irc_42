/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFuncs.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:40:19 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/18 22:24:26 by mgoltay          ###   ########.fr       */
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
	this->users.insert(std::pair<int, Client *>(invitee.getSocketFd(), &invitee));
	if (c)
		broadcast(*c, "*invited " + invitee.getNickname() + " to the channel*");
	else
		broadcast(invitee, "*joined the channel*");
}

void	Channel::mode(Client *c, bool sign, char mode, std::string parameter)
{
	if (mode == 'i')
		this->inviteonly = sign;
	else if (mode == 't')
		this->trestrict = sign;
	else if (mode == 'k' && !sign)
		this->password = "";
	else if (mode == 'k' && sign)
		this->password = parameter; // ! needs fixing and broadcast to ops
	c->sendmsg(RED "Include Channel Name!" RESET "\n");
	// TODO IMPLEMENT THE MODES
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
