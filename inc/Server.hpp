/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:15:26 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/14 15:54:59 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Server
{
	private:
		int								sfd;
		int								counter;
		bool							shutdown;
		std::string						joinpass;
		std::string						operpass;
		std::vector<pollfd>				clientfds;
		std::map<int, Client> 			clients;
		std::map<int, Client *> 		operators;
		std::map<std::string, Channel>	channels;

	public:
		// *----- Orthodox Canonical Form -----
		Server( void );
		Server( const Server &f );
		Server &operator=( const Server &f );
		~Server( void );

		// *----- Attribute Getters -----
		int								getFdSize(void);
		int								getCounter(void);
		bool							getShutdown(void);
		std::string						getPassword(void) const;
		std::string						getOperPass(void) const;
		Client							*getClient(int cfd);
		Client							*getClientNick(std::string nick);
		Client							*getClientFd(int fd);
		Client							*getClientUser(std::string user);
		std::map<int, Client>			&getClients(void);
		Channel							*getChannel(std::string name);
		std::map<std::string, Channel*>	getChannels(Client *c);
		std::map<std::string, Channel>	&getChannels(void);
		
		// *----- Attribute Setters -----
		bool	isUser(Client &c);
		bool	isOp(Client &c);
		void	setShutDown(bool set);
		void	setPassword(std::string pass);
		void	removeUser(int cfd);
		void	setOperPass(std::string pass);
		void	addOperator(Client *potop);
		void	addChannel(std::string name, Client &c);
		void	removeChannel(std::string name);
		void	decrementCounter(void);


		// *---- Server Functions (ServerFuncs.cpp) ----
		bool		countDown(void);
		void		clientDisconnect(std::string reason, int cfd);
		void		deliverToClient(Client &client);
		int			HandleClients( void );
		int			appendpollfd(int new_socket);
		int			accept_connect( void );
		int			assign(char *portstr, char *pass);
		int			bootup(char *portstr, char *pass);

};
