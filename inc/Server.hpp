/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:15:26 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/01 16:21:18 by mgoltay          ###   ########.fr       */
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
		struct sockaddr_in				addr;
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

		// *----- Attribute getters -----
		int								getFd(void);
		int								getCounter(void);
		bool							getShutdown(void);
		std::string						getPassword(void) const;
		std::string						getOperPass(void) const;
		Client							*getClient(int cfd);
		Client							*getClientNick(std::string nick);
		Client							*getClientUser(std::string user);
		std::map<int, Client>			&getClients(void);
		Channel							*getChannel(std::string name);
		std::map<std::string, Channel>	&getChannels(void);
		
		// *----- Attribute setters -----
		bool	isUser(Client &c);
		bool	isOp(Client &c);
		void	setShutDown(bool set);
		void	setPassword(std::string pass);
		void	removeUser(int cfd);
		void	setOperPass(std::string pass);
		void	addOperator(Client *potop);
		void	addChannel(std::string name, Client &c);
		void	removeChannel(std::string name);
		void	print(void);
		void	decrementCounter(void);


		// *---- ServerFuncs.cpp ----
		void		countDown(void);
		void		deliverToClient(Client &client);
		int			HandleClients( void );
		int			accept_connect( void );
		int			appendpollfd(int new_socket);
		int			assign(char *portstr, char *pass);
		int			bootup(char *portstr, char *pass);

};
