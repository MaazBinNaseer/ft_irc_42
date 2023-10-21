/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:29:30 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/21 12:34:25 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

typedef struct s_cap
{
	bool	echo_msg; // sends msg back when you send it to client or channel
	bool	ext_join; // prints real name when joining channel
	bool	inv_notif;	// notifies clients of channel someone joined
}	t_cap;

class Client
{
    private:
	
		unsigned int				_socket_fd;
		unsigned int				_client_id;
		std::string					_hostname;
		std::string					_username;
		std::string					_nickname;
		std::string					_realname;
		bool						_pass;
		bool						_registered;
		std::string					_receiveBuffer;
		std::vector<std::string> 	_sendBuffer;
		t_cap						_caps;

    public:

		bool						_cap_order;
		Client(unsigned int fd, std::string hostname);
	
        // *----- Orthodox Canonical Form -----
		Client();
		Client( const Client &f );
		Client & operator=(Client const & rhs);
        ~Client();

		// *------ Attribute getters ------
		int							getSocketFd(void) const ;
		int							getClientId(void) const ;
		std::string					getUsername(void) const ;
		std::string					getHostname(void) const ;
		std::string					getNickname(void) const ;
		std::string					getRealname(void) const ;
		bool						getPass(void) const ;
		bool						getRegistered(void) const ;
		std::string					&getReceiveBuffer(void);
		std::vector<std::string>	&getSendBuffer(void);
		t_cap						getCaps(void) const ;

		// *------ Attribute setters ------
		void						setSocketFd(int fd);
		void						setClientId(int id);
		void						setUsername(std::string username);
		void						setHostname(std::string nickname);
		void						setNickname(std::string nickname);
		void						setRealname(std::string nickname);
		void						setPass(bool set);
		void						setRegistered(bool set);

		// *------ Other functions ------
		void						fillInfo(int fd, std::string username, std::string nickname);
		void						postInfo(void);
		void						sendmsg(std::string msg);
		void						print(std::string color);
};
