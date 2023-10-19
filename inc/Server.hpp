/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:15:26 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/18 21:43:15 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"



class Server
{
	private:
		int	sfd;
		struct sockaddr_in				addr;
		std::string						password;
		std::vector<pollfd>				clientfds;
		std::map<int, Client> 			clients;
		std::map<std::string, Channel> 	channels;
		struct capabilities
		{
			bool _extended_join; //* Extends the JOIN message to include the account name of the joining client.
			bool _echo_message; //* Notifies clients when their PRIVMSG and NOTICEs are correctly received by the server.
			bool _invite_notify; //* Notifies clients when other clients are invited to common channels.
		};
		
	public:
		Server( void );
		Server( const Server &f );
		Server &operator=(const Server &f );
		~Server( void );

		int								getFd(void);
		std::string						getPassword(void);
		Client							*getClient(int cfd);
		Client							*getClientNick(std::string nick);
		Client							*getClientUser(std::string user);
		std::map<int, Client>			&getClients(void);
		Channel							*getChannel(std::string name);
		std::map<std::string, Channel>	&getChannels(void);

		void							setPassword(std::string pass);
		void							addChannel(std::string name, Client &c);
		void							print(void);

	//*	ServerFuncs.cpp
		int								appendpollfd(int new_socket);
		int								assign(char *portstr, char *pass);
		int								accept_connect( void );
		void							HandleParse( int i );
		int								HandleClients( void );
		int								bootup(char *portstr, char *pass);
};
