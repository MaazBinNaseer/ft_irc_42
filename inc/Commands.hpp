#pragma once

#include "ft_irc.hpp"

//* Condition Check Macros

#define PARAM		'P'
#define REGIS		'R'
#define PASSW		'W'
#define CHECK		'c'
#define REQUIRED	'r'
#define LEN			'L'
#define NICKNAME	'n'
#define USERNAME	'u'
#define USERNICK	'N'
#define NONICK		'n'
#define NOFOUND		'f'
#define INUSEN		'i'
#define INUSECH		'h'

class Commands : public Parse
{
	private:

		typedef void (Commands::*actions)(void);
		std::map<std::string, actions> 	_selection;

		bool _order;
		bool _multiple;

		void	checkConditions(std::string flags);
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
		void 	EXIT(void);

	public:
		//* Canonical Orthodox Form
		Commands();
		Commands(Client *req_client, Server *srvptr);
		Commands(Client *req_client, Server *srvptr, std::string &buff);
		~Commands();

		//* Command Execution Setup
		void	setAttributes( void );
		bool	toRegister( std::string command );
		void	executeCommand( void );

		//* Complementary Functions
		void 	handleMultiple(std::string comm);
		void	parseMode(void);
		std::string	concArgs(int start);
};
