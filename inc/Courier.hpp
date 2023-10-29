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

class NeedMoreParams: public std::exception
{
	private:

	const char *	_no_of_params;

	public:

	NeedMoreParams(std::string params);
	const char * what() const throw();
};

// *----- Log Commands -----
void logStart(std::ofstream &log);
void logConnect(int fd, std::ofstream &log);
void logDisconnect(std::string reason, int fd, std::ofstream &log);
void logRecv(std::string input, int fd, std::ofstream &log);
void logSend(std::deque<std::string> messages, int fd, std::ofstream &log);
void logEnd(std::ofstream &log);
void serverLog(Client &acted, Client &target, std::string cmd, std::string msg);

// *----- Messages Sent to Clients -----
void selfCommand(Client &acted, std::string cmd, std::string msg);
void targettedCommand(Client &acted, Client &target, std::string cmd, std::string msg);
void serverMessage(std::string code, std::string message, Client &client);
void welcomeMessage(Client &client);