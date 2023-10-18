#include "../inc/ft_irc.hpp"

void Commands::CAP(void)
{
	std::string command1 = getCmdArg(0);
	std::cout << "Commands from CAP is called " << command1 << std::endl;
	if (command1 == "LS")
		this->_req_client->sendmsg(GREEN "CAP * LIST: chghost, server_time, account_tag, extended_join, invite_notify\r" RESET "\n");
	this->_req_client->postInfo();
}

	// this->_req_client->postInfo();

void Commands::PASS(void)
{
	if (this->_serv->getPassword() == getCmdArg(0))
		_req_client->sendmsg(GREEN "Connected! Welcome to our Server! :)" RESET "\n");
	else
	{
		_req_client->sendmsg(RED "Incorrect Password! Termination Imminent!" RESET "\n");
		close(_req_client->getSocketFd());
	}
}

void Commands::PING(void)
{

}

void Commands::NICK(void)
{
	if (this->_serv->getClientNick(getCmdArg(0)) && this->_serv->getClientNick(getCmdArg(0)) != this->_req_client)
		_req_client->sendmsg(RED "Nickname Taken! Choose another!" RESET "\n");
	else if (this->_serv->getChannel(getCmdArg(0)))
		_req_client->sendmsg(RED "Cannot set Channel Name as Nickname! Choose another!" RESET "\n");
	else
		this->_req_client->setNickname(getCmdArg(0));
}

void Commands::USER(void)
{
	std::string user = getCmdArg(0);
	if (this->_serv->getClientUser(user))
		_req_client->sendmsg(RED "Username Taken! Choose another!" RESET "\n");
	else if (user != "" && user[0] != ':')
		this->_req_client->setUsername(user);
	for (unsigned long i = 0; i < this->_cmd_args.size() && getCmdArg(i) != ""; i++)
	{
		if (getCmdArg(i)[0] == ':')
		{
			user = concArgs(i);
			user.erase(0, 1);
			this->_req_client->setRealname(user);
			break ;
		}
	}
}

void Commands::OPER(void)
{

}

void Commands::QUIT(void)
{
	if (getCmdArg(0) == "")
		this->_req_client->sendmsg(RED "Exiting Server! See you soon!" RESET "\n");
	else
		this->_req_client->sendmsg(RED + concArgs(0) + RESET + "\n");
	close(this->_req_client->getSocketFd());
}

void Commands::JOIN(void)
{
	// TODO IMPLEMENT THE MULTIPLE SELECTION VIA COMMAS ','		<>{,<>}

	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (targetch)
	{
		if (targetch->isInviteOnly())
			_req_client->sendmsg(RED "Channel is Invite Only!" RESET "\n");
		else if (targetch->getPassword() != "" && targetch->getPassword() != getCmdArg(1))
			_req_client->sendmsg(RED "Wrong Channel Password!" RESET "\n");
		else
			targetch->invite(NULL, *_req_client);
	}
	else if (this->_serv->getClientNick(getCmdArg(0)))
		_req_client->sendmsg(RED "Nickname Exists! Cannot Create Channel!" RESET "\n");
	else
		this->_serv->addChannel(getCmdArg(0), *_req_client);
}

void Commands::PART(void)
{
	// TODO IMPLEMENT THE MULTIPLE SELECTION VIA COMMAS ','		<>{,<>}

	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (targetch && targetch->exists(*this->_req_client))
		targetch->kick(NULL, *this->_req_client);
	
	// ! CHANGE IT SO MASTERS CAN LEAVE USING PART BUT IT DELETES IF EMPTY OR GIVES SOMEONE ELSE MASTER
}

void Commands::KICK(void)
{
	// TODO IMPLEMENT THE MULTIPLE SELECTION VIA COMMAS ','		<>{,<>}

	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(1));
	if (getCmdArg(0) == "")
		_req_client->sendmsg(RED "Enter Channel to Kick From!" RESET "\n");
	else if (!targetch)
		_req_client->sendmsg(RED "Channel not Found!" RESET "\n");
	else if (!targetch->isOp(*this->_req_client))
		_req_client->sendmsg(RED "You are not an Operator of this Channel!" RESET "\n");
	else if (getCmdArg(1) == "")
		_req_client->sendmsg(RED "Enter User to Kick!" RESET "\n");
	else if (!targetcl)
		_req_client->sendmsg(RED "User does not Exist!" RESET "\n");
	else if (!targetch->exists(*targetcl))
		_req_client->sendmsg(RED "User is not Part of this Channel!" RESET "\n");
	else if (targetcl == targetch->getMaster())
		_req_client->sendmsg(RED "User is a Master Operator!" RESET "\n");
	else
		targetch->kick(this->_req_client, *targetcl);
}

void Commands::INVITE(void)
{
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	Channel *targetch = this->_serv->getChannel(getCmdArg(1));
	if (getCmdArg(0) == "")
		_req_client->sendmsg(RED "Enter User to be Invited!" RESET "\n");
	else if (!targetcl)
		_req_client->sendmsg(RED "User not Found!" RESET "\n");
	else if (getCmdArg(1) == "")
		_req_client->sendmsg(RED "Enter Channel to be Invited to!" RESET "\n");
	else if (!targetch && this->_serv->getClientNick(getCmdArg(1)))
		_req_client->sendmsg(RED "Cannot invite User to another User!" RESET "\n");
	else if (!targetch)
		this->_serv->addChannel(getCmdArg(1), *targetcl);
	else if (targetch && targetch->isInviteOnly() && !targetch->isOp(*_req_client))
		_req_client->sendmsg(RED "Only Channel Operators can invite to channel '" + getCmdArg(1) + "!" RESET "\n");
	else
		targetch->invite(this->_req_client, *targetcl);
}

void Commands::TOPIC(void)
{
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (getCmdArg(0) == "")
		this->_req_client->sendmsg(RED "Include Channel Name!" RESET "\n");
	else if (!targetch)
		this->_req_client->sendmsg(RED "Channel '" + getCmdArg(0) + "' does not exist!" RESET "\n");
	else if (getCmdArg(1) == "")
		this->_req_client->sendmsg(PURPLE "[" + getCmdArg(0) + "] " YELLOW + targetch->getTopic() + RESET "\n");
	else if (!targetch->exists(*this->_req_client)) // * EVERYTHING BELOW IS ABOUT SETTING TOPIC
		this->_req_client->sendmsg(RED "Cannot set Topic of channel you are not part of!" RESET "\n");
	else if (targetch->hasTopicRestrictions() && !targetch->isOp(*this->_req_client))
		this->_req_client->sendmsg(RED "Setting Topic restricted to Channel Operators!" RESET "\n");
	else
		targetch->setTopic(this->_req_client, concArgs(1));
}

void	fill(std::string options, char flags[5], bool present[5])
{
	for (unsigned long i = 1; i < options.size(); i++)
		for (int j = 0; j < 5; j++)
			if (options[i] == flags[j])
				present[j] = true;
}

void Commands::MODE(void)
{
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (getCmdArg(0) == "")
		this->_req_client->sendmsg(RED "Include Channel Name!" RESET "\n");
	else if (!targetch)
		this->_req_client->sendmsg(RED "Channel '" + getCmdArg(0) + "' does not exist!" RESET "\n");
	
	std::string options = getCmdArg(1);
	char	flags[6] = "itkol";
	bool	present[5] = {false, false, false, false, false};
	fill(options, flags, present);

	if (options == "")
		this->_req_client->sendmsg(RED "Input Channel's Mode!" RESET "\n");
	else if (options[0] != '-' || options[0] != '+')
		this->_req_client->sendmsg(RED "Specify direction of mode! (+ or -)" RESET "\n");
	else if ((present[2] && present[3]) || (present[3] && present[4]) || (present[2] && present[4]))
		this->_req_client->sendmsg(RED "Cannot Use Modes - k,o,l - Together!" RESET "\n");
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
	if (str.back() == ' ')
		str.back() = '\0';
	return (str);
}

void Commands::PRIVMSG(void)
{
	// TODO IMPLEMENT THE MULTIPLE SELECTION VIA COMMAS ','		<>{,<>}

	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	if (targetch && targetch->exists(*this->_req_client))
		targetch->broadcast(*_req_client, concArgs(1));
	else if (targetch)
		_req_client->sendmsg(RED "Join channel '" + targetch->getName() + "' to send message!" RESET "\n");
	else if (!targetcl)
		_req_client->sendmsg(RED "No User or Channel of that Name Exists!" RESET "\n");
	else
		targetcl->sendmsg(PURPLE "[PRIV] " GREEN + _req_client->getNickname() + ": " YELLOW + concArgs(1) + RESET "\n");
}

void Commands::NOTICE(void)
{
	PRIVMSG();
	// ! DIFFERENCE: automatic replies must never be sent in response
}

void Commands::WHOIS(void)
{
	// TODO IMPLEMENT THE MULTIPLE SELECTION VIA COMMAS ','		<>{,<>}
	std::map<std::string, Channel> &channels = this->_serv->getChannels();

	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	if (getCmdArg(0) == "")
		_req_client->sendmsg(RED "Enter NickName of Query!" RESET "\n");
	else if (!targetcl)
		_req_client->sendmsg(RED "User not Found!" RESET "\n");
	else
	{
		_req_client->sendmsg(YELLOW "_____________________________________" RESET "\n");
		_req_client->sendmsg(GREEN "Username: " + targetcl->getUsername() + RESET "\n");
		_req_client->sendmsg(GREEN "Hostname: " + targetcl->getHostname() + RESET "\n");
		_req_client->sendmsg(GREEN "Nickname: " + targetcl->getNickname() + RESET "\n");
		_req_client->sendmsg(GREEN "Realname: " + targetcl->getRealname() + RESET "\n");
		_req_client->sendmsg(CYAN "Channels: " RESET "\n");
		for (std::map<std::string, Channel>::iterator it=channels.begin(); it != channels.end(); it++)
			if (it->second.exists(*targetcl))
				_req_client->sendmsg(PURPLE + it->first + RESET "\n");
		_req_client->sendmsg(YELLOW "_____________________________________" RESET "\n");
	}

	// PRINT LIST OF CHANNELS PART OF
}

void Commands::KILL(void)
{

}

Commands::Commands()
{

}

Commands::Commands(Client *req_client, Server *srvptr): Parse(req_client, srvptr)
{
	_selection["CAP"] = &Commands::CAP;				// TODO
	_selection["PASS"] = &Commands::PASS;			// !
	_selection["PING"] = &Commands::PING;			// TODO
	_selection["NICK"] = &Commands::NICK;			// DONE
	_selection["USER"] = &Commands::USER;			// DONE
	_selection["OPER"] = &Commands::OPER;			// TODO
	_selection["QUIT"] = &Commands::QUIT;			// DONE
	_selection["JOIN"] = &Commands::JOIN;			// TODO
	_selection["PART"] = &Commands::PART;			// !
	_selection["KICK"] = &Commands::KICK;			// DONE
	_selection["INVITE"] = &Commands::INVITE;		// DONE
	_selection["TOPIC"] = &Commands::TOPIC;			// DONE
	_selection["MODE"] = &Commands::MODE;			// *
	_selection["PRIVMSG"] = &Commands::PRIVMSG;		// DONE
	_selection["NOTICE"] = &Commands::NOTICE;		// !
	_selection["WHOIS"] = &Commands::WHOIS;			// DONE
	_selection["KILL"] = &Commands::KILL;			// TODO
}

Commands::~Commands()
{

}

void	Commands::executeCommand()
{
	std::map<std::string, actions>::iterator select;

	// ! Consider upper casing the first command
	select = this->_selection.find(this->_cmd);
	if (select != this->_selection.end())
		(this->*select->second)();
	else
		this->_req_client->sendmsg(RED "Invalid Command '" + getCmd() + "'!" RESET "\n");
}