/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 14:34:04 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/20 20:09:36 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Channel {
	private:
		const std::string		name;
		std::string				topic;
		std::string				password;
		std::map<int, Client *>	users;
		std::map<int, Client *>	ops;
		bool					inviteonly;
		bool					trestrict;
		int						userlimit;
		bool					isBotChannel;
	
	public:
		// *----- Canonical Orthodox Form -----
		Channel( void );
		Channel(std::string n, Client &c);
		Channel(std::string n, bool isBotchannel);
		Channel( const Channel &f );
		Channel &operator=( const Channel &f );
		~Channel( void );

		// *----- Attribute Getters -----
		std::string				getName( void ) const ;
		std::string				getTopic( void ) const;
		std::string				getPassword( void ) const ;
		std::map<int, Client *> getUsers( void ) const ;
		std::map<int, Client *> getOps( void ) const ;
		bool					isInviteOnly( void ) const ;
		bool					isEmpty() const;
		bool					hasTopicRestrictions( void ) const;
		int						getSize( void );
		Client					*getClientNick(std::string nick);
		
		// *----- Attribute Setters -----
		void	setTopic(Client *c, std::string t); // *	NULL if simply display
	
		// *----- Channel Functions (ChannelFuncs.cpp) -----
		bool	exists(Client &c);
		bool	isOp(Client c);
	
		void	broadcast(Client &c, std::string cmd, std::string msg);
		void	broadcastOps(Client *c, std::string msg);
		
		int		kick(Client *c, Client &kickee, std::string command); // if last client removed delete this channel
		void	invite(Client *c, Client &invitee, std::string command);

		void	handleO(Client *c, bool sign, std::string parameter, std::string modes);
		void	handleL(Client *c, bool sign, std::string parameter, std::string modes);
		void	mode(Client *c, bool sign, char mode, std::string parameter);

		void	print(void);
};
