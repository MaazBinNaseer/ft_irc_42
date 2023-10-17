/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 14:34:04 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/17 20:59:29 by mgoltay          ###   ########.fr       */
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
		Client					*master;
		bool					inviteonly;
		bool					trestrict;
		int						userlimit;

	public:
		Channel(std::string n, Client &c);

		Channel( void );
		Channel( const Channel &f );
		Channel &operator=( const Channel &f );
		~Channel( void );

		std::string				getName( void ) const ;
		std::string				getTopic( void ) const;
		std::string				getPassword( void ) const ;
		std::map<int, Client *> getUsers( void ) const ;
		std::map<int, Client *> getOps( void ) const ;
		Client					*getMaster( void ) const ;
		bool					isInviteOnly( void ) const ;
		int						getSize( void );
		
		void	setMaster(Client &c);
		void	setTopic(std::string t); // *	NULL if simply display
	
		bool	exists(Client c);
	
		void	kick(Client &kickee); // if last client removed delete this channel
		void	invite(Client &invitee);
		void	mode(bool sign, char mode, std::string *parameters);

		void	broadcast(Client &c, std::string msg);
		void	print(void);
		void	botfuncs(); 
		/*	eg:
		 *	detects curse word, kicks client
		 *	could spit facts to keep server interesting
		 */
};

// channel names dont have:
// spaces		' '	(32)
// control G	^G	(7)
// commas		','	(44)

// channel operators have @ when referenced