#pragma once

#include "ft_irc.hpp"

class Commands : public Parse
{
	private:

	typedef void (Commands::*actions)(void);
	std::map<std::string, actions> 	_selection;
	
	bool _order;
	bool _multiple;

	//* Complementary Functions
	void 	handleMultiple(std::string comm);
	void	parseMode(void);
	std::string	concArgs(int start);

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
	void	MSG(void);
	void	PRIVMSG(void);
	void	NOTICE(void); // ! send notices between users and channels

	//* User Based Queries
	void	WHOIS(void); // ! get information about a user

	//* Operator Messages
	void	KILL(void); // ! disconnect users from the server
	void 	EXIT(void);
	public:
		Commands();
		Commands(Client *req_client, Server *srvptr);
		Commands(Client *req_client, Server *srvptr, std::string &buff);
		~Commands();

		void	setAttributes( void );
		bool	toRegister( std::string command );
		void	executeCommand( void );

		void	print();
};
