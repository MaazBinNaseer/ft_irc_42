#include "../inc/ft_irc.hpp"

//* ====== Authentication Commands

void Commands::CAP(void)
{
	std::string command1 = getCmdArg(0);
	if (command1 == "LS")
    {
		std::string message = "CAP * LS :echo_msg extended_join invite_notify\r\n";
        this->_req_client->_cap_order = true;
        this->_req_client->pushSendBuffer(message);
    }
    if(this->_req_client->_cap_order && command1 == "REQ")
	{
		std::string strCaps[3] = {"echo_msg", "extended_join", "invite_notify"};
		std::string message = "CAP * ACK :";
		for (unsigned long i = 1; !getCmdArg(i).empty(); i++)
			for (unsigned long j = 0; j < 3; j++)
				if (getCmdArg(i) == strCaps[j])
				{
					this->_req_client->setCaps(j, true);
					message += getCmdArg(i) + " ";
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
	clock_t startTime = clock();
	_req_client->sendmsg(GREEN "PONG " + getCmdArg(0) + ": " RESET);
	clock_t endTime = clock();
	double elapsedTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;
	std::ostringstream message;
	message << std::fixed << std::setprecision(6);
    message << "Time taken to process PING and send PONG: " << elapsedTime << " seconds\r\n";
    selfCommand(*_req_client, "PING", GREEN + message.str() + RESET);
	// _req_client->sendmsg(GREEN + message.str() + RESET);
}

//* ====== User Assigning Commands

void Commands::NICK(void)
{
	checkConditions("WrP1LnNiNh");
	this->_req_client->setNickname(getCmdArg(0));
	if (_req_client->getReceiveBuffer().empty())
		selfCommand(*_req_client, "NICK", GREEN "Nickname set!" RESET);
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
	this->_req_client->setUsername(getCmdArg(0));
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
	std::map<int, Client> clients = this->_serv->getClients();
	serverLog(*_req_client, targetcl->getNickname(), "Turned target into an operator");
	serverMessage(RPL_YOUREOPER, GREEN "You are now an IRC operator" RESET, *targetcl);
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		Client *broad = this->_serv->getClientNick(it->second.getNickname());
		if (broad != targetcl)
			broadcastallCommand(*broad, *targetcl, this->_cmd, ":" GREEN "is now an IRC operator!" RESET);
	}
}

void Commands::QUIT(void)
{
	if (this->_cmd_args.size() == 0)
		selfCommand(*_req_client, this->_cmd, "leaving");
	else
		selfCommand(*_req_client, this->_cmd, concArgs(0));
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
	checkConditions("CiCpClNiLc");
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (targetch)
		targetch->invite(NULL, *_req_client);
	else
	{
		checkConditions("Nh");
		selfCommand(*_req_client, "JOIN", GREEN "You have made channel: " + getCmdArg(0) + RESET);
		this->_serv->addChannel(getCmdArg(0), *_req_client);
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
	checkConditions("CeCn");
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (targetch->kick(NULL, *this->_req_client))
		this->_serv->removeChannel(getCmdArg(0));
}

void Commands::KICK(void)
{
	if (!this->_multiple)
	{
		checkConditions("P2");
		handleMultiple("KICK");
	}
	checkConditions("CeCoNd");
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(1));
	if (targetch && targetcl && !targetch->exists(*targetcl))
		throw CommandError("User Not In Channel", ERR_USERNOTINCHANNEL, targetcl->getNickname() + " is not in " + targetch->getName() + " channel", *_req_client);
	else if (targetcl && targetcl != _req_client)
	{
		if (getCmdArg(2) != "" && concArgs(2).size() <= KICKLEN)
			selfCommand(*targetcl, "KICK", "You are being kicked because " + concArgs(2));
		serverLog(*_req_client, targetcl->getNickname(), "Has kicked the target user");
		targetch->kick(this->_req_client, *targetcl);
	}
}

void Commands::INVITE(void)
{
	checkConditions("P2NfCxCa");
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	Channel *targetch = this->_serv->getChannel(getCmdArg(1));
	if (!targetch && this->_serv->getClientNick(getCmdArg(1)))
		_req_client->sendmsg(RED "Cannot invite User to another User!" RESET "\n");
	else if (!targetch)
		this->_serv->addChannel(getCmdArg(1), *targetcl);
	else if (targetch && targetch->isInviteOnly() && !targetch->isOp(*_req_client))
		selfCommand(*_req_client, "INVITE", "Only Channel Operators can invite to channel '" + getCmdArg(1) + "'!");
	else
		targetch->invite(this->_req_client, *targetcl);
}

void Commands::TOPIC(void)
{
	checkConditions("P1CeCn");
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (getCmdArg(1) == "")
		selfCommand(*_req_client, "TOPIC" , PURPLE "[" + getCmdArg(0) + "] " GREEN "TOPIC= " YELLOW + targetch->getTopic() + RESET);
	else if (targetch->hasTopicRestrictions() && !targetch->isOp(*this->_req_client))
		throw CommandError("Operator Privilege Needed", ERR_CHANOPRIVSNEEDED, "Setting Topic restricted to Channel Operators", *_req_client);
	else
	{
		targetch->setTopic(this->_req_client, concArgs(1));
		selfCommand(*_req_client, "TOPIC", GREEN "You have set the topic: " + concArgs(1) + RESET);
	}
}

void Commands::MODE(void)
{
	checkConditions("P2CeCo");
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
		targetch->broadcast(*_req_client, "PRIVMSG", concArgs(1));
	else if (targetch)
		throw CommandError("Channel Incompatibility", ERR_CANNOTSENDTOCHAN, "Join channel '" + targetch->getName() + "' to send messages", *_req_client);
	else if (targetcl)
	{
		targettedCommand(*_req_client, *targetcl, "PRIVMSG", PURPLE "[PRIV] " GREEN + _req_client->getNickname() + " :" YELLOW + concArgs(1) + RESET);
		if (!this->_multiple && this->_req_client->getCaps().echo_msg)
			selfCommand(*_req_client, "PRIVMSG", PURPLE "[PRIV] " GREEN + _req_client->getNickname() + " :" YELLOW + concArgs(1) + RESET);
		return ;
	}
	checkConditions("NfCe");
}

void Commands::NOTICE(void)
{
	PRIVMSG();
	// ! DIFFERENCE: automatic replies must never be sent in response
}

//* ====== User Based Queries

void Commands::WHOIS(void)
{
	if (!this->_multiple)
	{
		checkConditions("P1");
		handleMultiple("WHOIS");
	}
	checkConditions("Nf");
	std::map<std::string, Channel> &channels = this->_serv->getChannels();
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	// if (getCmdArg(0) == "")
	// 	_req_client->sendmsg(RED "Enter NickName of Query!" RESET "\n");
	// else if (!targetcl)
	// 	_req_client->sendmsg(RED "User not Found!" RESET "\n");
	// else
	// {
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
		// _req_client->sendmsg(YELLOW "_____________________________________" RESET "\n");
		// _req_client->sendmsg(GREEN "Username: " + targetcl->getUsername() + RESET "\n");
		// _req_client->sendmsg(GREEN "Hostname: " + targetcl->getHostname() + RESET "\n");
		// _req_client->sendmsg(GREEN "Nickname: " + targetcl->getNickname() + RESET "\n");
		// _req_client->sendmsg(GREEN "Realname: " + targetcl->getRealname() + RESET "\n");
		// _req_client->sendmsg(CYAN "Channels: " RESET "\n");
		// for (std::map<std::string, Channel>::iterator it=channels.begin(); it != channels.end(); it++)
		// 	if (it->second.exists(*targetcl))
		// 		_req_client->sendmsg(PURPLE + it->first + RESET "\n");
		// _req_client->sendmsg(YELLOW "_____________________________________" RESET "\n");
	// }
}

void Commands::KILL(void)
{
	checkConditions("NoP1Nf");
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	if (getCmdArg(1) != "")
		targettedCommand(*_req_client, *targetcl, "KILL", PURPLE "["+ this->_req_client->getNickname() + "] " YELLOW + "has kicked you because: " CYAN + concArgs(1) + RESET);
		// targetcl->sendmsg(PURPLE "["+ this->_req_client->getNickname() + "] " YELLOW + "has kicked you because: " CYAN + concArgs(1) + RESET + "\n");
	this->_serv->removeUser(targetcl->getSocketFd());
	std::map<int, Client> &clients = this->_serv->getClients();
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
		selfCommand(it->second, "KILL", PURPLE "["+ this->_req_client->getNickname() + "] " YELLOW "has removed " RED + getCmdArg(0) + "!" RESET);
		// it->second.sendmsg(PURPLE "["+ this->_req_client->getNickname() + "] " YELLOW "has removed " RED + getCmdArg(0) + "!" RESET "\n");
}

void Commands::EXIT(void)
{
	checkConditions("No");
	std::map<int, Client> clients = this->_serv->getClients();
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		Client *broad = this->_serv->getClientNick(it->second.getNickname());
		selfCommand(*broad, "EXIT",  YELLOW "Server is shutting down" RESET);
	}
	this->_serv->setShutDown(true);
}

//* ====== Canonical Orthodox Form

Commands::Commands()
{
	setAttributes();
}

Commands::Commands(Client *req_client, Server *srvptr): Parse(req_client, srvptr)
{
	setAttributes();
}

Commands::Commands(Client *req_client, Server *srvptr, std::string &buff): Parse(req_client, srvptr)
{
	setAttributes();

	size_t pos = buff.find_first_of('\n');
	if (pos == std::string::npos)
		return ;
	std::string cmd = buff.substr(0, pos + 1);
	buff = buff.substr(pos + 1);
	trim(cmd);
	std::cout << PURPLE << cmd << RESET << "\n"; // ! DEBUGGING
	if (cmd.empty())
		return ;
	this->_cmd = extractWord(cmd);
	std::transform(this->_cmd.begin(), this->_cmd.end(), this->_cmd.begin(), ::toupper);
	while (!cmd.empty())
		this->_cmd_args.push_back(extractWord(cmd));
	executeCommand();
}

Commands::~Commands()
{

}

//* ====== Command Execution Setup

void	Commands::setAttributes()
{
	this->_order = false;
	this->_multiple = false;
	_selection["CAP"] = &Commands::CAP;				// ?
	_selection["PASS"] = &Commands::PASS;			// DONE
	_selection["PING"] = &Commands::PING;			// ?
	_selection["NICK"] = &Commands::NICK;			// DONE
	_selection["USER"] = &Commands::USER;			// DONE
	_selection["OPER"] = &Commands::OPER;			// DONE
	_selection["QUIT"] = &Commands::QUIT;			// DONE
	_selection["JOIN"] = &Commands::JOIN;			// DONE
	_selection["PART"] = &Commands::PART;			// DONE
	_selection["KICK"] = &Commands::KICK;			// DONE
	_selection["INVITE"] = &Commands::INVITE;		// DONE
	_selection["TOPIC"] = &Commands::TOPIC;			// DONE
	_selection["MODE"] = &Commands::MODE;			// DONE
	_selection["PRIVMSG"] = &Commands::PRIVMSG;		// DONE
	_selection["NOTICE"] = &Commands::NOTICE;		// ! AUTO
	_selection["WHOIS"] = &Commands::WHOIS;			// DONE
	_selection["KILL"] = &Commands::KILL;			// DONE
	_selection["EXIT"] = &Commands::EXIT;
}

bool	Commands::toRegister(std::string command)
{
	if (command != "CAP" && command != "PASS" && command != "NICK" && command != "USER" && command != "QUIT")
		return(_req_client->getRegistered());
	return (true);
}


void	Commands::executeCommand()
{
	std::map<std::string, actions>::iterator select;

	select = this->_selection.find(this->_cmd);
	if (toRegister(this->_cmd))
	{
		if (select != this->_selection.end())
		{
			try
			{
				(this->*select->second)();
			}
			catch(std::exception &e)
			{
				std::string error(e.what());
				serverLog(*_req_client, "", error + " Error Caught\n");
				std::cout << RED << e.what() << " Error Caught" << RESET << std::endl;
			}
		}
		else
			serverMessage(ERR_UNKNOWNCOMMAND, RED + this->_cmd + " Unknown command" RESET, *_req_client);
	}
	else
		serverMessage(ERR_NOTREGISTERED, RED "Need to register first using PASS <password>, NICK <nickname> then USER <username> <hostname> <servername> <realname>" RESET, *_req_client);
}

//* ====== Complementary Functions

void Commands::handleMultiple(std::string comm)
{
	this->_multiple = true;
	std::string str = getCmdArg(0);
	size_t pos = str.find_first_of(",");
	while (pos != std::string::npos)
	{
		this->_cmd_args[0] = str.substr(0, pos);
		try 
		{
			if (this->_cmd_args[0].empty())
				throw CommandError("Empty Arguments (double commas)", ERR_UNKNOWNCOMMAND, "Empty argument (double commas)", *_req_client);
			(this->*_selection[comm])();
		} 
		catch(std::exception &e)
		{
			std::string error(e.what());
			serverLog(*_req_client, "", error + " Error Caught\n");
			std::cout << RED << e.what() << " Error Caught" << RESET << std::endl;
		}
		str = str.substr(pos + 1);
		pos = str.find_first_of(",");
	}
	this->_cmd_args[0] = str;
	this->_multiple = false;
	return ;
}

void	fill(std::string options, char flags[5], bool present[5])
{
	for (unsigned long i = 1; i < options.size(); i++)
		for (int j = 0; j < 5; j++)
			if (options[i] == flags[j])
				present[j] = true;
}

void	Commands::parseMode(void)
{
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	std::string options = getCmdArg(1);
	char	flags[6] = "itkol";
	bool	present[5] = {false, false, false, false, false};
	fill(options, flags, present);

	if (options == "")
		selfCommand(*_req_client, "", RED "Input Channel's Mode!" RESET);
		// this->_req_client->sendmsg(RED "Input Channel's Mode!" RESET "\n");
	else if (options[0] != '-' && options[0] != '+')
		selfCommand(*_req_client, "", RED "Specify direction of mode! (+ or -)" RESET);
		// this->_req_client->sendmsg(RED "Specify direction of mode! (+ or -)" RESET "\n");
	else if ((present[2] && present[3]) || (present[3] && present[4]) || (present[2] && present[4]))
		selfCommand(*_req_client, "", RED "Cannot Use Modes - k,o,l - Together!" RESET);
		// this->_req_client->sendmsg(RED "Cannot Use Modes - k,o,l - Together!" RESET "\n");
	else
		for (int i = 0; i < 5; i++)
			if (present[i])
				targetch->mode(this->_req_client, options[0] == '+', flags[i], getCmdArg(2));
}

std::string Commands::concArgs(int start)
{
	std::string str = "";
	for (std::vector<std::string>::iterator it = _cmd_args.begin() + start; it != _cmd_args.end(); it++)
		str += *(it) + " ";
	if (!str.empty() && str[str.size() - 1] == ' ')
        str.erase(str.size() - 1, 1);
	return (str);
}
