#pragma once

#include "ft_irc.hpp"
#include <exception>

#define TRIPLE_INFO(nick, user, host) (nick + "[!" + user + "@" + host + "]")

#define S " "
#define DASH "----------------------------------------\n"
#define CORNER_UP "/----------------------------------------\\\n";
#define CORNER_DOWN "\\----------------------------------------/\n";

class FailedFunction: public std::exception
{
	private:
	
	const char *	_msg;
	
	public:
	
	FailedFunction(std::string function);
	const char * what() const throw();
	
};

class CommandError: public std::exception
{
	private:

	const char * type_of_error;

	public:

	CommandError(const char *type, std::string code, std::string message, Client &client);
	const char * what() const throw();
};

// *----- Log Commands -----
void logStart();
void logConnect(int fd);
void logDisconnect(std::string reason, int fd);
void logRecv(std::string input, int fd);
void logSend(std::deque<std::string> messages, int fd);
void logRegister(Client &client);
void logEnd();
void serverLog(Client &acted, std::string target, std::string note);

// *----- Messages Sent to Clients -----
void selfCommand(Client &acted, std::string cmd, std::string msg);
void targettedCommand(Client &acted, Client &target, std::string cmd, std::string msg);
void serverMessage(std::string code, std::string message, Client &client);
void welcomeMessage(Client &client);