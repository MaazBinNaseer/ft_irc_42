/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:15:26 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/26 13:12:48 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Server
{
	private:
		int								sfd;
		int								counter;
		std::string						joinpass;
		std::string						operpass;
		std::vector<pollfd>				clientfds;
		std::map<int, Client> 			clients;
		std::map<int, Client *> 		operators;
		std::map<std::string, Channel>	channels;
		Bot								botInstance;

	public:
		// *----- Orthodox Canonical Form -----
		Server( void );
		Server( const Server &f );
		Server &operator=( const Server &f );
		~Server( void );

		// *----- Attribute Getters -----
		int								getFdSize(void);
		int								getCounter(void);
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
		Bot								&getBot();
		
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
		void	createBotChannel();


		// *---- Server Functions (ServerFuncs.cpp) ----
		bool		countDown(void);
		void		clientDisconnect(std::string reason, int cfd);
		void		deliverToClient(Client &client);
		int			HandleClients( void );
		int			appendpollfd(int new_socket, bool is_server);
		int			accept_connect( void );
		int			assign(char *portstr, char *pass);
		int			bootup(char *portstr, char *pass);

		void		print();
};
