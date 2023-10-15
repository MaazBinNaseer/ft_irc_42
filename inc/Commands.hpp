#pragma once

#include "ft_irc.hpp"

class Commands
{
	protected:

	//* Authentication Commands
	void	CAP(void);
	void	PASS(void);
	void	PING(void); // ! Ping pong to check connection

	//* User Assigning Commands
	void	NICK(void);
	void	USER(void);
	void	OPER(void);// ! give operator priveleges
	void	QUIT(void);// ! user quits the server

	//* Channel Related Commands
	void	JOIN(void);
	void	PART(void);// ! remove users from channels
	void	KICK(void);
	void	INVITE(void);
	void	TOPIC(void);
	void	MODE(void);

	//* Sending Messages
	void	PRIVMSG(void);
	void	NOTICE(void); // ! send notices between users and channels

	//* User Based Queries
	void	WHOIS(void); // ! get information about a user

	//* Operator Messages
	void	KILL(void); // ! disconnect users from the server

};
