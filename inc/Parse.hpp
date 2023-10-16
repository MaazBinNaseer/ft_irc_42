#pragma once

#include "ft_irc.hpp"

class Parse
{
	protected:

		std::string 					_cmd;
		std::vector<std::string>		_cmd_args;
		Client							*_req_client;
		Server							*_serv;

	public:

		// *----- Orthodox Canonical Form -----
		Parse();
		Parse(Client *req_client, Server *srvptr);
		Parse(Parse const & copy);
		Parse & operator=(Parse const & rhs);
		~Parse();

		// *----- Attribute getters and printers -----
		std::string getCmd(void);
		std::string	getCmdArg(unsigned long i);
		void		printCmdArgs(void);
		int			getClientFd(void);
		Client		*getReqClient(void);
		void		printClientData(Client *reqclient);

		// *----- Parser -----
		void		trim(std::string &buffer);
		void		assignCommand(std::string &buffer);
		void		assignArguments(std::string &buffer);

};
