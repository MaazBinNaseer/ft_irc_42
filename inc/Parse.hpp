#pragma once

#include "ft_irc.hpp"

class Parse : public Commands
{
	private:

		typedef void (Parse::*actions)(void);
		std::map<std::string, actions> 	_selection;
		std::string 					_cmd;
		std::vector<std::string>		_cmd_args;
		int								_client_fd;
		Client							*_req_client;

	public:

		// *----- Orthodox Canonical Form -----
		Parse();
		Parse(int client_fd, Client &req_client);
		Parse(Parse const & copy);
		Parse & operator=(Parse const & rhs);
		~Parse();

		// *----- Attribute getters and printers -----
		std::string getCmd(void);
		void		printCmdArgs(void);
		int			getClientFd(void);
		Client		*getReqClient(void);
		void		printClientData(Client *reqclient);

		// *----- Parser -----
		void		trim(std::string &buffer);
		void		assignCommand(std::string &buffer);
		void		assignArguments(std::string &buffer);

		// *----- Executor -----
		void		executeCommand(void);
};
