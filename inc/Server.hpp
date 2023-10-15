/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:15:26 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/15 19:48:21 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Server
{
	private:
		int	sfd;
		struct sockaddr_in	addr;
		std::string			password;
		std::vector<pollfd>	clientfds;
		std::map<int, Client> clients;
		std::map<std::string, Channel> channels;

	public:
		Server( void );
		Server( const Server &f );
		Server &operator=( const Server &f );
		~Server( void );

		int	getFd(pollfd poll);

		int	appendpollfd(int new_socket);
		int	assign(char *portstr, char *pass);
		int	accept_connect( void );
		void HandleParse( int i );
		int HandleClients( void );

		int	bootup(char *portstr, char *pass);
};
