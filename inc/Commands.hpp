#pragma once

#include "ft_irc.hpp"

//* Condition Check Macros

#define _CHANNEL	'C'
	#define EXIST		'e'
	#define EXIST2		'x'
	#define INVITEO		'i'
	#define PASSWORDC	'p'
	#define LIMIT		'l'
	#define NOTPART		'n'
	#define NOTPART2	'a'
	#define OPERATORC	'o'
#define _LEN		'L'
	#define NICKNAMEL	'n'
	#define USERNAMEL	'u'
	#define CHANNELL	'c'
	#define _PARAM		'P'
#define _NICKNAME	'N'
	#define NONICK		'n'
	#define NOFOUND0	'f'
	#define NOFOUND1	'd'
	#define INUSEN		'i'
	#define INUSECH		'h'
	#define OPERATORN	'o'
#define _PASSWORD	'W'
	#define CHECK		'c'
	#define REQUIRED	'r'
	#define OPERATOR	'o'
#define _REGIS		'R'

class Commands : public Parse
{
	private:
		std::string 					_cmd;
		std::vector<std::string>		_cmd_args;
		Client							*_req_client;
		Server							*_serv;

		typedef void (Commands::*actions)(void);
		std::map<std::string, actions> 	_selection;

		bool _order;
		bool _multiple;

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

		// *----- Parser -----
		void		trim(std::string &buffer);
		std::string	extractWord(std::string &buffer);

	public:
		//* Canonical Orthodox Form
		Commands();
		Commands(Client *req_client, Server *srvptr);
		Commands(Client *req_client, Server *srvptr, std::string &buff);
		~Commands();

		// *----- Attribute getters and printers -----
		std::string getCmd(void);
		std::string	getCmdArg(unsigned long i);

		//* Command Execution Setup
		void	setAttributes( void );
		bool	toRegister( std::string command );
		void	executeCommand( void );

		//* Complementary Functions
		void 		handleMultiple(std::string comm);
		void		parseMode(void);
		std::string	concArgs(int start);

		//* Conditions Checkers
		void		checkPassword(char flag);
		void		checkNickname(char flag);
		void		checkLen(char flag);
		void		checkChannel(char flag);
		void		checkConditions(std::string flags);
};
