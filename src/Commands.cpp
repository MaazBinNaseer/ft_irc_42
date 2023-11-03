#include "../inc/ft_irc.hpp"

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
	_selection["CAP"] = &Commands::CAP;				// ? Whats Left ?
	_selection["PASS"] = &Commands::PASS;
	_selection["PING"] = &Commands::PING;			// ? Whats Left ?
	_selection["NICK"] = &Commands::NICK;
	_selection["USER"] = &Commands::USER;
	_selection["OPER"] = &Commands::OPER;
	_selection["QUIT"] = &Commands::QUIT;
	_selection["JOIN"] = &Commands::JOIN;
	_selection["PART"] = &Commands::PART;
	_selection["KICK"] = &Commands::KICK;
	_selection["INVITE"] = &Commands::INVITE;
	_selection["TOPIC"] = &Commands::TOPIC;
	_selection["MODE"] = &Commands::MODE;
	_selection["PRIVMSG"] = &Commands::PRIVMSG;
	_selection["NOTICE"] = &Commands::NOTICE;		// ! AUTO MSGS HANDLE???
	_selection["WHOIS"] = &Commands::WHOIS;
	_selection["KILL"] = &Commands::KILL;
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
			serverMessage(ERR_UNKNOWNCOMMAND, this->_cmd + " :Unknown command", *_req_client);
	}
	else
		serverMessage(ERR_NOTREGISTERED, ":need to register first using PASS <password>, NICK <nickname> then USER <username> <hostname> <servername> <realname>", *_req_client);
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
		(this->*_selection[comm])();
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
		this->_req_client->sendmsg(RED "Input Channel's Mode!" RESET "\n");
	else if (options[0] != '-' && options[0] != '+')
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
	if (!str.empty() && str[str.size() - 1] == ' ')
        str.erase(str.size() - 1, 1);
	return (str);
}

//* ====== Authentication Commands

void Commands::CAP(void)
{
	std::string command1 = getCmdArg(0);
	if (command1 == "LS")
    {
		std::string message = "CAP * LS :chghost server_time account_tag extended_join invite_notify\r\n";
        this->_req_client->_cap_order = true;
        this->_req_client->pushSendBuffer(message);
    }
    if(this->_req_client->_cap_order && command1 == "REQ")
	{
		std::string message = "CAP * ACK :" + getCmdArg(1) + "\r\n";
        this->_req_client->pushSendBuffer(message);
	}

}

void Commands::PASS(void)
{
	if (_req_client->getPass())
		throw CommandError("Already Registered", ERR_ALREADYREGISTERED, "No need to reregister", *_req_client);
	else if (this->_cmd_args.size() < 1)
		throw CommandError("Insufficient Parameters", ERR_NEEDMOREPARAMS, this->_cmd + " Not enough parameters", *_req_client);
	else if (this->_serv->getPassword() == getCmdArg(0))
	{
		this->_req_client->sendmsg(GREEN "Correct Password!" RESET "\n");
		_req_client->setPass(true);
	}
	else
	{
		serverMessage(ERR_PASSWDMISMATCH, ":Password incorrect", *_req_client);
		_req_client->setRemove(true);
		_req_client->setReason("Inputted incorrect password");
	}
}

void Commands::PING(void)
{
	if(this->_cmd_args.size() < 1)
		throw CommandError("Insufficient Parameters", ERR_NEEDMOREPARAMS, this->_cmd + " :Not enough parameters", *_req_client);
	else if (_req_client->getNickname() == "*")
		throw CommandError("No Nickname Given", ERR_NONICKNAMEGIVEN, ":Nickname required to register use PING <nickname>", *_req_client);
	else if (this->_serv->getClientNick(getCmdArg(0)) && this->_serv->getClientNick(getCmdArg(0)) != this->_req_client)
		throw CommandError("Nickname In Use", ERR_NICKNAMEINUSE, getCmdArg(0) + " :Nickname is already in use by another user", *_req_client);
	clock_t startTime = clock();
	_req_client->sendmsg(GREEN "PONG " + getCmdArg(0) + ": " RESET);
	clock_t endTime = clock();
	double elapsedTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;
	std::ostringstream message;
	message << std::fixed << std::setprecision(6);
    message << "Time taken to process PING and send PONG: " << elapsedTime << " seconds\r\n";
    _req_client->sendmsg(GREEN + message.str() + RESET); // ! check if error
}

//* ====== User Assigning Commands

void Commands::NICK(void)
{
	if (this->_cmd_args.size() < 1)
		throw CommandError("Insufficient Parameters", ERR_NEEDMOREPARAMS, this->_cmd + " Not enough parameters", *_req_client);
	else if (!_req_client->getPass())
		throw CommandError("Password Required", ERR_PASSWDMISMATCH, "Password needed", *_req_client);
	else if (getCmdArg(0).size() > NICKLEN)
		throw CommandError("Nickname Too Long", ERR_NOSUCHNICK, getCmdArg(0) + " Nickname is too long", *_req_client); // ! FIX EXIT CODE
	else if (this->_serv->getClientNick(getCmdArg(0)) && this->_serv->getClientNick(getCmdArg(0)) != this->_req_client)
		throw CommandError("Nickname In Use", ERR_NICKNAMEINUSE, getCmdArg(0) + " Nickname is already in use by another user", *_req_client);
	else if (this->_serv->getChannel(getCmdArg(0)))
		throw CommandError("Nickname In Use", ERR_NICKNAMEINUSE, getCmdArg(0) + " Nickname is already in use by a channel", *_req_client);
	else
	{
		this->_req_client->setNickname(getCmdArg(0));
		this->_req_client->sendmsg(GREEN "Nickname set!" RESET "\n");
	}
}

void Commands::USER(void)
{
	if (_req_client->getRegistered())
		throw CommandError("Already Registered", ERR_ALREADYREGISTERED, "No need to reregister", *_req_client);
	else if (!_req_client->getPass())
		throw CommandError("Password Required", ERR_PASSWDMISMATCH, "Password needed", *_req_client);
	else if (this->_cmd_args.size() < 4)
		throw CommandError("Insufficient Parameters", ERR_NEEDMOREPARAMS, this->_cmd + " Not enough parameters", *_req_client);
	else if (_req_client->getNickname() == "*")
		throw CommandError("No Nickname Given", ERR_NONICKNAMEGIVEN, "Nickname required to register using NICK <nickname>", *_req_client);
	else if (this->_serv->getClientUser(getCmdArg(0)))
		throw CommandError("Username already exists", ERR_ALREADYREGISTERED, "Username Already Exists", *_req_client); // ! FIX EXIT CODE
	else if (getCmdArg(0).size() > USERLEN)
		throw CommandError("Username Too Long", ERR_NOSUCHNICK, getCmdArg(0) + " Username is too long", *_req_client); // ! FIX EXIT CODE
	else if (getCmdArg(3)[0] != ':')
		throw CommandError("No Colon For Real Name", ERR_NEEDMOREPARAMS, "USER needs ':' for Realname", *_req_client); // ! FIX EXIT CODE

	_req_client->setUsername(getCmdArg(0));
	_req_client->setHostname(getCmdArg(1));
	_req_client->setServername(getCmdArg(2));
	std::string user = concArgs(3);
	user.erase(0, 1);
	this->_req_client->setRealname(user);	
	_req_client->setRegistered(true);
	logRegister(*_req_client);
	welcomeMessage(*_req_client, *_serv);
}

void Commands::OPER(void)
{
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	if (getCmdArg(0) == "" || getCmdArg(1) == "")
		throw CommandError("Insufficient Parameters", ERR_NEEDMOREPARAMS, "User And Password Needed for Operator Privileges!", *_req_client);
	else if (!targetcl)
		throw CommandError("User Not Found", ERR_NOSUCHNICK, "User does not Exist!", *_req_client);
	else if (getCmdArg(1) != this->_serv->getOperPass())
		throw CommandError("Incorrect Password", ERR_PASSWDMISMATCH, "Incorrect Password for Operator Privileges!", *_req_client);

	this->_serv->addOperator(targetcl);
	std::map<int, Client> clients = this->_serv->getClients();
	serverLog(*_req_client, targetcl->getNickname(), "Turned target into an operator");
	serverMessage(RPL_YOUREOPER, "You are now an IRC operator", *targetcl);
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
		if (it->first != targetcl->getSocketFd())
			broadcastallCommand(it->second, *targetcl, this->_cmd, ":is now an IRC operator!");
}

void Commands::QUIT(void)
{
	if (this->_cmd_args.size() == 0)
		selfCommand(*_req_client, this->_cmd, "leaving");
		// this->_req_client->sendmsg(RED "Exiting Server! See you soon!" RESET "\n");
	else
		selfCommand(*_req_client, this->_cmd, concArgs(0));
		// this->_req_client->sendmsg(RED "You have Quit Because: " + concArgs(0) + RESET + "\n");
	_req_client->setRemove(true);
	_req_client->setReason(concArgs(0));
	// this->_serv->removeUser(this->_req_client->getSocketFd());
}

//* ====== Channe Related Commands

void Commands::JOIN(void)
{
	if (!this->_multiple)
		handleMultiple("JOIN");

	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (targetch)
	{
		if (targetch->isInviteOnly())
			_req_client->sendmsg(RED "Channel is Invite Only!" RESET "\n");
		else if (targetch->getPassword() != "" && targetch->getPassword() != getCmdArg(1))
			_req_client->sendmsg(RED "Wrong Channel Password!" RESET "\n");
		else
		{
			if (this->_serv->getChannels(this->_req_client).size() >= CHANLIMIT)
				throw CommandError("Channel Limit Reached", ERR_NOSUCHNICK, "Channel Limit Reached", *_req_client); // !WRONG ERROR
			targetch->invite(NULL, *_req_client);
		}
	}
	else if (this->_serv->getClientNick(getCmdArg(0)))
		_req_client->sendmsg(RED "Nickname Exists! Cannot Create Channel!" RESET "\n");
	else if (getCmdArg(0).size() > CHANNELEN)
		throw CommandError("Channel Name Too Long", ERR_NOSUCHNICK, getCmdArg(0) + " Channel Name is too long", *_req_client); // !WRONG ERROR
	else
	{
		this->_req_client->sendmsg(GREEN "You have made Channel: " + getCmdArg(0) + RESET "\n");
		this->_serv->addChannel(getCmdArg(0), *_req_client);
	}
}

void Commands::PART(void)
{
	if (!this->_multiple)
		handleMultiple("PART");

	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (getCmdArg(0) == "")
		this->_req_client->sendmsg(RED "Enter Channel to Part From!" RESET "\n");
	else if (!targetch || !targetch->exists(*this->_req_client))
		this->_req_client->sendmsg(RED "You are not part of a channel '" + getCmdArg(0) + "'!" RESET "\n");
	else if (targetch->kick(NULL, *this->_req_client))
		this->_serv->removeChannel(getCmdArg(0));
}

void Commands::KICK(void)
{
	if (!this->_multiple)
		handleMultiple("KICK");

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
	else
	{
		if (getCmdArg(2) != "" && concArgs(2).size() <= KICKLEN)
			targetcl->sendmsg(RED "You are being kicked because " + concArgs(2) + RESET "\n");
		targetch->kick(this->_req_client, *targetcl);
	}
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
		this->_req_client->sendmsg(PURPLE "[" + getCmdArg(0) + "] " GREEN "TOPIC= " YELLOW + targetch->getTopic() + RESET "\n");
	else if (!targetch->exists(*this->_req_client))
		this->_req_client->sendmsg(RED "Cannot set Topic of channel you are not part of!" RESET "\n");
	else if (targetch->hasTopicRestrictions() && !targetch->isOp(*this->_req_client))
		this->_req_client->sendmsg(RED "Setting Topic restricted to Channel Operators!" RESET "\n");
	else
	{
		targetch->setTopic(this->_req_client, concArgs(1));
		this->_req_client->sendmsg(GREEN "You have set the Topic!" RESET "\n");
	}
}

void Commands::MODE(void)
{
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (getCmdArg(0) == "")
		this->_req_client->sendmsg(RED "Include Channel Name!" RESET "\n");
	else if (!targetch)
		this->_req_client->sendmsg(RED "Channel '" + getCmdArg(0) + "' does not exist!" RESET "\n");
	else if (!targetch->isOp(*_req_client))
		this->_req_client->sendmsg(RED "You are not an Operator of this Channel!" RESET "\n");
	else
		parseMode();
}

//* ====== Sending Messages

void Commands::PRIVMSG(void)
{
	if (!this->_multiple)
		handleMultiple("PRIVMSG");

	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	if (targetch && targetch->exists(*this->_req_client))
		targetch->broadcast(*_req_client, concArgs(1));
	else if (targetch)
		_req_client->sendmsg(RED "Join channel '" + targetch->getName() + "' to send message!" RESET "\n");
	else if (!targetcl)
		_req_client->sendmsg(RED "No User or Channel of that Name Exists!" RESET "\n");
	else
	{
		targetcl->sendmsg(PURPLE "[PRIV] " GREEN + _req_client->getNickname() + ": " YELLOW + concArgs(1) + RESET "\n");
		if (!this->_multiple && this->_req_client->getCaps().echo_msg)
			this->_req_client->sendmsg(PURPLE "[PRIV] " GREEN + _req_client->getNickname() + ": " YELLOW + concArgs(1) + RESET "\n");
	}
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
		handleMultiple("WHOIS");

	std::map<std::string, Channel> &channels = this->_serv->getChannels();

	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (getCmdArg(0) == "")
		_req_client->sendmsg(RED "Enter Nick/Name of Query!" RESET "\n");
	else if (!targetcl && !targetch)
		_req_client->sendmsg(RED "Entity not Found!" RESET "\n");
	else if (targetch)
	{
		_req_client->sendmsg(YELLOW "_____________________________________" RESET "\n");
		_req_client->sendmsg(GREEN "Channel Name: " + targetch->getName() + RESET "\n");
		_req_client->sendmsg(GREEN "Channel Topic: " + targetch->getTopic() + RESET "\n");
		_req_client->sendmsg(BLUE "Channel Operators: " RESET "\n");
		std::map<int, Client *>	cmap = targetch->getOps();
		for (std::map<int, Client *>::iterator it = cmap.begin(); it != cmap.end(); it++)
			_req_client->sendmsg(PURPLE + it->second->getNickname() + RESET "\n");
		_req_client->sendmsg(BLUE "Channel Regular Users: " RESET "\n");
		cmap = targetch->getUsers();
		for (std::map<int, Client *>::iterator it = cmap.begin(); it != cmap.end(); it++)
			if (!targetch->isOp(*it->second))
				_req_client->sendmsg(CYAN + it->second->getNickname() + RESET "\n");
		_req_client->sendmsg(YELLOW "_____________________________________" RESET "\n");
	}
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
}

void Commands::KILL(void)
{
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	if (!this->_serv->isOp(*this->_req_client))
		_req_client->sendmsg(RED "Only an IRC operator can exeute KILL!" RESET "\n");
	else if (getCmdArg(0) == "")
		_req_client->sendmsg(RED "Enter NickName of KILL victim!" RESET "\n");
	else if (!targetcl)
		_req_client->sendmsg(RED "Victim does not Exist!" RESET "\n");
	else
	{
		if (getCmdArg(1) != "")
			targetcl->sendmsg(PURPLE "["+ this->_req_client->getNickname() + "] " YELLOW + "has kicked you because: " CYAN + concArgs(1) + RESET + "\n");
		this->_serv->removeUser(targetcl->getSocketFd());
		std::map<int, Client> &clients = this->_serv->getClients();
		for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
			it->second.sendmsg(PURPLE "["+ this->_req_client->getNickname() + "] " YELLOW "has removed " RED + getCmdArg(0) + "!" RESET "\n");
	}
}

void Commands::EXIT(void)
{
	std::map<int, Client> clients = this->_serv->getClients();
	if (!this->_serv->isOp(*this->_req_client))
		throw CommandError("Privileges Required", ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator", *_req_client);
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		Client *broad = this->_serv->getClientNick(it->second.getNickname());
		selfCommand(*broad, "EXIT",  YELLOW "Server is shutting down" RESET "\r\n");
	}
	this->_serv->setShutDown(true);
}
