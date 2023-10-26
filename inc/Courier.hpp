#pragma once

#include "ft_irc.hpp"
#include <exception>

#define TRIPLE_INFO(nick, user, host) (nick + "[!" + user + "@" + host + "]")

#define S " "

class FailedFunction: public std::exception
{
	private:
	
	const char *	_msg;
	
	public:
	
	FailedFunction(std::string function);
	const char * what() const throw();
	
};

class NeedMoreParams: public std::exception
{
	private:

	const char *	_no_of_params;

	public:

	NeedMoreParams(std::string params);
	const char * what() const throw();
};

void serverLog(Client &acted, Client &target, std::string cmd, std::string msg);
void targettedCommand(Client &acted, Client &target, std::string cmd, std::string msg);
void serverMessage(std::string code, std::string message, Client &client);
void welcomeMessage(Client &client);