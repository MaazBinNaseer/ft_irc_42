/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelFuncs.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 20:40:19 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/17 21:06:33 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

bool Channel::exists(Client c)
{
	return (this->users.find(c.getSocketFd()) != this->users.end());
}

void	Channel::broadcast(Client &c, std::string msg)
{
	std::string newmsg = PURPLE + getName() + ": " + GREEN + c.getNickname() + ": " + YELLOW + msg + RESET + "\n";

	std::map<int, Client *>::iterator it;
	for (it = users.begin(); it != users.end(); it++)
		it->second->sendmsg(newmsg);
}

void	Channel::kick(Client &kickee)
{
	if (!exists(kickee) || kickee.getSocketFd() == this->master->getSocketFd())
		return ;
	this->users.erase(kickee.getSocketFd());
	if (this->ops.find(kickee.getSocketFd()) != this->ops.end())
		this->ops.erase(kickee.getSocketFd());
}

void	Channel::invite(Client &invitee)
{
	if (!exists(invitee))
		this->users.insert(std::pair<int, Client *>(invitee.getSocketFd(), &invitee));
	// * broadcast to all ops
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
