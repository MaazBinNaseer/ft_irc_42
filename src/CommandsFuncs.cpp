/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandsFuncs.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:18:03 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/26 20:34:21 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

//* ====== Authentication Commands

void Commands::CAP(void)
{
	std::string command1 = getCmdArg(0);
	if (command1 == "LS")
	{
		std::string message = "CAP * LS :echo-message extended-join invite-notify\r\n";
		this->_req_client->_cap_order = true;
		this->_req_client->pushSendBuffer(message);
	}
	if(this->_req_client->_cap_order && command1 == "REQ")
	{
		std::string strCaps[3] = {"echo-message", "extended-join", "invite-notify"};
		std::string message = "CAP * ACK :";
		for (unsigned long i = 0; !getCmdArg(i).empty(); i++)
		{
			std::string cap = getCmdArg(i);
			for (unsigned long j = 0; j < 3; j++)
			{
				if (cap.at(0) == ':')
					cap.erase(0, 1);
				if (cap == strCaps[j])
				{
					this->_req_client->setCaps(j, true);
					message += cap + " ";
				}
			}
		}
		message += "\r\n";
		this->_req_client->pushSendBuffer(message);
	}

}

void Commands::PASS(void)
{
	checkConditions("RP1Wc");
	if (_req_client->getReceiveBuffer().empty())
		selfCommand(*_req_client, "PASS", GREEN "Correct Password!" RESET);
	_req_client->setPass(true);
}

void Commands::PING(void)
{
	checkConditions("P1");
	// clock_t startTime = clock();
	customMessage(*_req_client, "PONG :" + getCmdArg(0));
	// clock_t endTime = clock();
	// double elapsedTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC * 1000000;
	// std::ostringstream message;
	// message << std::fixed << std::setprecision(6);
	// message << GREEN "Time taken to process PING and send PONG: " << elapsedTime << " microseconds" RESET;
	// customMessage(*_req_client, message.str());
}

//* ====== User Assigning Commands

void Commands::NICK(void)
{
	checkConditions("WrP1LnNiNh");
	if (getCmdArg(0).at(0) == '#')
		throw CommandError("Forbidden Nickname", ERR_UNKNOWNERROR, "Nickname not allowed (starts with '#')", *_req_client);
	selfCommand(*_req_client, "NICK " + getCmdArg(0), GREEN "Nickname set!" RESET);
	this->_req_client->setNickname(getCmdArg(0));
}

void Commands::USER(void)
{
	checkConditions("WrNnRP4Lu");
	if (getCmdArg(3)[0] == ':')
	{
		std::string user = concArgs(3);
		user.erase(0, 1);
		this->_req_client->setRealname(user);
	}
	else
		throw CommandError("No Colon For Real Name", ERR_NEEDMOREPARAMS, "USER needs ':' for Realname", *_req_client);
	this->_req_client->setUsername("~" + getCmdArg(0));
	if (this->_req_client->getHostname().empty())
		this->_req_client->setHostname(getCmdArg(1));
	this->_req_client->setServername(getCmdArg(2));
	this->_req_client->setRegistered(true);
	logRegister(*_req_client);
	welcomeMessage(*_req_client, *_serv);
}

void Commands::OPER(void)
{
	checkConditions("P2NfWo");
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	this->_serv->addOperator(targetcl);
	std::map<int, Client> &clients = this->_serv->getClients();
	serverLog(*_req_client, targetcl->getNickname(), "Turned target into an operator");
	serverMessage(RPL_YOUREOPER, GREEN "You are now an IRC operator" RESET, *targetcl);
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
		if (it->second.getSocketFd() != targetcl->getSocketFd())
			broadcastallCommand(it->second, *targetcl, this->_cmd, ":" GREEN + targetcl->getNickname() + " is now an IRC operator!" RESET);
}

void Commands::QUIT(void)
{
	if (this->_cmd_args.size() == 0)
		selfCommand(*_req_client, this->_cmd, PURPLE "leaving" RESET);
	else
		selfCommand(*_req_client, this->_cmd, PURPLE + concArgs(0) + RESET);
	
	for (std::map<std::string, Channel>::iterator it = _serv->getChannels().begin(); it != _serv->getChannels().end(); it++)
	{
		if (it->second.kick(this->_req_client, *this->_req_client, "PART") && it->second.getName() != "#bot")
		{
			std::string name = it->first;
			it--;
			_serv->removeChannel(name);
		}
	}
	std::map<int, Client> &clients = this->_serv->getClients();
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
		broadcastallCommand(it->second, *_req_client, this->_cmd, PURPLE "Quit: " + concArgs(0) + RESET);
	selfCommand(*_req_client, this->_cmd, PURPLE "*Using QUIT*" RESET);
	_req_client->setRemove(true);
	_req_client->setReason(concArgs(0));
}

//* ====== Channe Related Commands

void Commands::JOIN(void)
{
	if (!this->_multiple)
	{
		checkConditions("P1");
		handleMultiple("JOIN");
	}
	checkConditions("H0CiCpClNiLc");
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (targetch)
	{
		targetch->invite(NULL, *this->_req_client, "JOIN");
		if (targetch->getName() == "#bot")
			_serv->getBot().BotIntroduction(_req_client);
	}
	else
	{
		checkConditions("Nh");
		messageCommand(*_req_client, getCmdArg(0), "JOIN", GREEN "You have made channel: " + getCmdArg(0) + RESET);
		this->_serv->addChannel(getCmdArg(0), *_req_client);
		selfCommand(*_req_client, "PRIVMSG " + getCmdArg(0), YELLOW "Welcome, please add a topic with TOPIC <channelname> <topic>" RESET);
		selfCommand(*_req_client, "332" S + _req_client->getNickname() + S + getCmdArg(0) , "[Set a topic]");
		selfCommand(*_req_client, "MODE" S + getCmdArg(0) + S "+o" S + _req_client->getNickname(), "First operator created");
		serverLog(*_req_client, getCmdArg(0), "has created the target channel");
	}
}

void Commands::PART(void)
{
	if (!this->_multiple)
	{
		checkConditions("P1");
		handleMultiple("PART");
	}
	checkConditions("H0CeCn");
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (targetch->kick(this->_req_client, *this->_req_client, "PART") && targetch->getName() != "#bot")
		this->_serv->removeChannel(getCmdArg(0));
	selfCommand(*_req_client, "PART " + getCmdArg(0), PURPLE "*Using PART*" RESET);
}

void Commands::KICK(void)
{
	if (!this->_multiple)
	{
		checkConditions("P2");
		handleMultiple("KICK");
	}
	checkConditions("H0CeCoNd");
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(1));
	if (targetch && targetcl && !targetch->exists(*targetcl))
		throw CommandError("User Not In Channel", ERR_USERNOTINCHANNEL, targetcl->getNickname() + " is not in " + targetch->getName() + " channel", *_req_client);
	else if (targetcl && targetcl != _req_client)
	{
		if (getCmdArg(2) != "" && concArgs(2).size() <= KICKLEN)
			selfCommand(*targetcl, "KICK", "You are being kicked because " + concArgs(2));
		serverLog(*_req_client, targetcl->getNickname(), "Has kicked the target user");
		targetch->kick(this->_req_client, *targetcl, "KICK");
	}
}

void Commands::INVITE(void)
{
	checkConditions("H1P2NfCxCa");
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	Channel *targetch = this->_serv->getChannel(getCmdArg(1));
	if (!targetch && this->_serv->getClientNick(getCmdArg(1)))
		customMessage(*_req_client, RED "Cannot invite User to another User!" RESET "\n");
	else if (targetch && targetch->isInviteOnly() && !targetch->isOp(*_req_client))
		selfCommand(*_req_client, "PRIVMSG " + _req_client->getNickname(), "Only Channel Operators can invite to channel '" + getCmdArg(1) + "'!");
	else
	{
		targetch->invite(this->_req_client, *targetcl, "INVITE");
		if (targetch->getName() == "#bot")
			_serv->getBot().BotIntroduction(targetcl);
	}
}

void Commands::TOPIC(void)
{
	checkConditions("H0P1CeCn");
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (getCmdArg(1) == "")
		selfCommand(*_req_client, "TOPIC" , PURPLE "[" + getCmdArg(0) + "] " GREEN "TOPIC= " YELLOW + targetch->getTopic() + RESET);
	else if (targetch->hasTopicRestrictions() && !targetch->isOp(*this->_req_client))
	{
		selfCommand(*_req_client, "PRIVMSG" S + targetch->getName(), RED "Setting Topic restricted to Channel Operators" RESET);
		throw CommandError("Operator Privilege Needed", ERR_CHANOPRIVSNEEDED, "Setting Topic restricted to Channel Operators", *_req_client);
	}
	else
	{
		targetch->setTopic(this->_req_client, concArgs(1));
		selfCommand(*_req_client, "TOPIC " + targetch->getName(), GREEN + concArgs(1) + RESET);
	}
}

void Commands::MODE(void)
{
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	if (targetcl)
		return ;
	checkConditions("CeCo");
	parseMode();
}

//* ====== Sending Messages

void Commands::PRIVMSG(void)
{
	if (!this->_multiple)
	{
		checkConditions("P1");
		handleMultiple("PRIVMSG");
	}
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	if (targetch && targetch->exists(*this->_req_client))
	{
		if(targetch->getName() == "#bot")
		{
				std::string botMessage = getCmdArg(1);
				botMessage = stripQuotes(botMessage);
				if(botMessage == "1")
					_serv->getBot().factBot(_req_client);
				else if(botMessage == "2")
					_serv->getBot().triviaBot(_req_client, getCmdArg(2));
		}
		else
			targetch->broadcast(*_req_client, "PRIVMSG", YELLOW + concArgs(1) + RESET);
		return ;
	}
	else if (targetch)
		throw CommandError("Channel Incompatibility", ERR_CANNOTSENDTOCHAN, "Join channel '" + targetch->getName() + "' to send messages", *_req_client);
	else if (targetcl)
	{
		if (getCmdArg(2) == "SEND")
			targettedCommand(*_req_client, *targetcl, "PRIVMSG" S + targetcl->getNickname(), concArgs(1));
		else
		{
			targettedCommand(*_req_client, *targetcl, "PRIVMSG" S + targetcl->getNickname(), YELLOW + concArgs(1) + RESET);
			if (!this->_multiple && this->_req_client->getCaps().echo_msg)
				selfCommand(*_req_client, "PRIVMSG" S + _req_client->getNickname(), PURPLE + concArgs(1) + RESET);
		}
		return ;
	}
	checkConditions("H0");
}

void Commands::NOTICE(void)
{
	PRIVMSG();
}

//* ====== User Based Queries

void Commands::WHOIS(void)
{
	if (DEBUG == 3)
		this->_serv->print();

	if (!this->_multiple)
	{
		checkConditions("P1");
		handleMultiple("WHOIS");
	}
	checkConditions("Nf");
	std::map<std::string, Channel> &channels = this->_serv->getChannels();
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	selfCommand(*_req_client, "WHOIS", YELLOW "_____________________________________" RESET);
	selfCommand(*_req_client, "WHOIS", GREEN "Username: " + targetcl->getUsername() + RESET);
	selfCommand(*_req_client, "WHOIS", GREEN "Hostname: " + targetcl->getHostname() + RESET);
	selfCommand(*_req_client, "WHOIS", GREEN "Nickname: " + targetcl->getNickname() + RESET);
	selfCommand(*_req_client, "WHOIS", GREEN "Realname: " + targetcl->getRealname() + RESET); // ? Should we show real name with this
	selfCommand(*_req_client, "WHOIS", CYAN "Channels: " RESET);
	for (std::map<std::string, Channel>::iterator it=channels.begin(); it != channels.end(); it++)
		if (it->second.exists(*targetcl))
			selfCommand(*_req_client, "WHOIS", PURPLE + it->first + RESET);
	selfCommand(*_req_client, "WHOIS", YELLOW "_____________________________________" RESET);
}

void Commands::KILL(void)
{
	checkConditions("NoP1Nf");
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	if (getCmdArg(1) != "")
		targettedCommand(*_req_client, *targetcl, "KILL", PURPLE "["+ this->_req_client->getNickname() + "] " YELLOW + "has kicked you because: " CYAN + concArgs(1) + RESET);
	targetcl->setRemove(true);
	targetcl->setReason("Removed using KILL command");
	std::map<int, Client> &clients = this->_serv->getClients();
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
		selfCommand(it->second, "KILL", PURPLE "["+ this->_req_client->getNickname() + "] " YELLOW "has removed " RED + getCmdArg(0) + "!" RESET);
}

void Commands::EXIT(void)
{
	checkConditions("No");
	std::map<int, Client> &clients = this->_serv->getClients();
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		selfCommand(it->second, "SQUIT", PURPLE "Server shutting down" RESET);
		selfCommand(it->second, "EXIT",  YELLOW "Server is shutting down" RESET);
	}
	this->_serv->setShutDown(true);
}