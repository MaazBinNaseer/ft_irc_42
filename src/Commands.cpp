#include "../inc/ft_irc.hpp"

void Commands::CAP(void)
{
	std::cout << "CAP command executed" << std::endl;
	this->_req_client->postInfo();
}

void Commands::PASS(void)
{

}

void Commands::PING(void)
{

}

void Commands::NICK(void)
{

}

void Commands::USER(void)
{

}

void Commands::OPER(void)
{

}

void Commands::QUIT(void)
{

}

void Commands::JOIN(void)
{
	std::string name = getCmdArg(0);
	Channel *targetch = this->_serv->getChannel(name);
	if (targetch)
	{
		if (targetch->isInviteOnly())
			_req_client->sendmsg(RED "Channel is Invite Only!" RESET "\n");
		else if (targetch->getPassword() != getCmdArg(1))
			_req_client->sendmsg(RED "Wrong Channel Password!" RESET "\n");
		else
			targetch->invite(*_req_client);
	}
	else
		this->_serv->addChannel(name, *_req_client);
}

void Commands::PART(void)
{

}

void Commands::KICK(void)
{

}

void Commands::INVITE(void)
{

}

void Commands::TOPIC(void)
{

}

void Commands::MODE(void)
{

}

std::string Commands::concArgs(void)
{
	std::string str = "";
	for (std::vector<std::string>::iterator it = _cmd_args.begin() + 1; it != _cmd_args.end(); it++)
		str += *(it) + " ";
	return (str);
}

void Commands::MSG(void)
{
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	if (targetch)
		targetch->broadcast(*_req_client, concArgs());
	else
		_req_client->sendmsg(RED "No channel of that name exists!\n" RESET);
}

void Commands::PRIVMSG(void)
{

}

void Commands::NOTICE(void)
{

}

void Commands::WHOIS(void)
{

}

void Commands::KILL(void)
{

}

Commands::Commands()
{

}

Commands::Commands(Client *req_client, Server *srvptr): Parse(req_client, srvptr)
{
	_selection["CAP"] = &Commands::CAP;
	_selection["PASS"] = &Commands::PASS;
	_selection["PING"] = &Commands::PING;
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
	_selection["MSG"] = &Commands::MSG;
	_selection["PRIVMSG"] = &Commands::PRIVMSG;
	_selection["NOTICE"] = &Commands::NOTICE;
	_selection["WHOIS"] = &Commands::WHOIS;
	_selection["KILL"] = &Commands::KILL;
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
		std::cout << "Command not found" << std::endl;
}