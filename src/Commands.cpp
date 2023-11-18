/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:36:27 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/18 14:22:41 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

// *----- Constructors/Destructors -----

Commands::Commands()
{
	setAttributes();
}

Commands::Commands(Client *req_client, Server *srvptr) : _req_client(req_client), _serv(srvptr)
{
	setAttributes();
}

Commands::Commands(Client *req_client, Server *srvptr, std::string &buff) : _req_client(req_client), _serv(srvptr)
{
	setAttributes();

	size_t pos = buff.find_first_of('\n');
	if (pos == std::string::npos)
		return ;
	std::string cmd = buff.substr(0, pos + 1);
	buff = buff.substr(pos + 1);
	this->trim(cmd);
	std::cout << PURPLE << cmd << RESET << "\n";
	if (cmd.empty())
		return ;
	this->_cmd = this->extractWord(cmd);
	capitalize(this->_cmd);
	while (!cmd.empty())
		this->_cmd_args.push_back(this->extractWord(cmd));
	this->executeCommand();
}

Commands::~Commands()
{

}

std::string	Commands::getCmd()
{
	return (this->_cmd);
}

std::string	Commands::getCmdArg(unsigned long i)
{
	if (i < this->_cmd_args.size())
		return (this->_cmd_args[i]);
	return ("");
}

void	Commands::trim(std::string &buffer)
{
	while (buffer[0] == ' ')
		buffer.erase(0, 1);
	for (unsigned long i = 0; i < buffer.size() - 1; i++)
	{
		if (buffer[i] == ' ' && buffer[i + 1] == ' ')
			buffer.erase(i--, 1);
		else if (buffer[i] == '"' && ++i)
			while (i < buffer.size() && buffer[i] != '"')
				i++;
		else if (buffer[i] == '\'' && ++i)
			while (i < buffer.size() && buffer[i] != '\'')
				i++;
	}
	while (buffer.size() > 1 && (buffer[buffer.size() - 2] == ' ' || buffer[buffer.size() - 2] == '\r'))
		buffer.erase(buffer.size() - 2, 1);
	buffer.erase(buffer.size() - 1, 1);
}

std::string	Commands::extractWord(std::string &buffer)
{
	size_t	pos = 0;
	while (pos < buffer.size())
	{
		if (buffer[pos] == '"' && ++pos)
			while (pos < buffer.size() && buffer[pos] != '"')
				pos++;
		else if (buffer[pos] == '\'' && ++pos)
			while (pos < buffer.size() && buffer[pos] != '\'')
				pos++;
		else if (buffer[pos] == ' ')
			break ;
		pos++;
	}
	while (buffer[pos] == ' ')
		buffer.erase(pos, 1);
	std::string ret = buffer.substr(0, pos);
	buffer.erase(0, pos);
	if (_req_client->getRegistered() && ret.at(0) == ':')
		ret.erase(0, 1);
	return (ret);
}

// *----- Command Execution Setup -----

void	Commands::setAttributes()
{
	this->_order = false;
	this->_multiple = false;
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
	if (select == this->_selection.end())
		serverMessage(ERR_UNKNOWNCOMMAND, RED + this->_cmd + " Unknown command" RESET, *_req_client);
	else if (!toRegister(this->_cmd))
		serverMessage(ERR_NOTREGISTERED, RED "Need to register first using PASS <password>, NICK <nickname> then USER <username> <hostname> <servername> <realname>" RESET, *_req_client);
	else
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
			if (!_req_client->getRegistered())
			{
				_req_client->setRemove(true);
				_req_client->setReason("Failure On Registeration");
			}
		}
	}
}

// *----- Complementary Functions -----

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

void	Commands::fill(std::string options, char flags[5], bool present[5])
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
	this->fill(options, flags, present);

	if (options == "")
		selfCommand(*_req_client, "", RED "Input Channel's Mode!" RESET);
	else if (options[0] != '-' && options[0] != '+')
		selfCommand(*_req_client, "", RED "Specify direction of mode! (+ or -)" RESET);
	else if ((present[2] && present[3]) || (present[3] && present[4]) || (present[2] && present[4]))
		selfCommand(*_req_client, "", RED "Cannot Use Modes - k,o,l - Together!" RESET);
	else
		for (int i = 0; i < 5; i++)
			if (present[i])
				targetch->mode(this->_req_client, options[0] == '+', flags[i], getCmdArg(2));
}

std::string Commands::concArgs(int start)
{
	std::string str = "";
	for (std::vector<std::string>::iterator it = _cmd_args.begin() + start; it != _cmd_args.end(); it++)
		str += *(it) + S;
	if (!str.empty() && str[str.size() - 1] == ' ')
        str.erase(str.size() - 1, 1);
	return (str);
}

std::string Commands::stripQuotes(const std::string& input)
{
 	std::string result;
    // Start from the first character if it's not a quote, otherwise from the second
    size_t start = input[0] == '"' ? 1 : 0;
    // End at the last character if it's not a quote, otherwise one before the last
    size_t end = input[input.size() - 1] == '"' ? input.size() - 1 : input.size();
    // Assign the substring without the quotes
    result.assign(input.begin() + start, input.begin() + end);
    return result;
}