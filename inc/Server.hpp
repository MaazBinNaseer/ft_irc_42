/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:15:26 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/29 17:20:46 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Server
{
	private:
		int	sfd;
		struct sockaddr_in	addr;
		std::string			joinpass;
		std::string			operpass;
		std::ofstream		log;
		std::vector<pollfd>	clientfds;
		std::map<int, Client> clients;
		std::map<int, Client *> operators;
		std::map<std::string, Channel> channels;

	public:
		// *----- Orthodox Canonical Form -----
		Server( void );
		Server( const Server &f );
		Server &operator=( const Server &f );
		~Server( void );

		// *----- Attribute getters -----
		int								getFd(void);
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
		void	setPassword(std::string pass);
		void	removeUser(int cfd);
		void	setOperPass(std::string pass);
		void	addOperator(Client *potop);
		void	addChannel(std::string name, Client &c);
		void	removeChannel(std::string name);
		void	print(void);


		// *---- ServerFuncs.cpp ----
		void	deliverToClient(Client &client);
		int		appendpollfd(int new_socket);
		int		assign(char *portstr, char *pass);
		int		accept_connect( void );
		int		HandleClients( void );

		int		bootup(char *portstr, char *pass);
};
