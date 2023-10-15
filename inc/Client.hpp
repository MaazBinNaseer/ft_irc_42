/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:29:30 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/15 18:55:02 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client
{
    private:
	
		unsigned int				_socket_fd;
		unsigned int				_client_id;
		std::string					_hostname;
		std::string					_username;
		std::string					_nickname;
		bool						_pass;
		bool						_registered;
		std::string					_receiveBuffer;
		std::vector<std::string> 	_sendBuffer;
		

    public:

        // *----- Orthodox Canonical Form -----
		Client();
		Client(unsigned int fd, std::string hostname);
		Client & operator=(Client const & rhs);
        ~Client();

		// *------ Attribute getters ------
		int							getSocketFd(void);
		int							getClientId(void);
		std::string					getUsername(void);
		std::string					getNickname(void);
		bool						getPass(void);
		bool						getRegistered(void);
		std::string					&getReceiveBuffer(void);
		std::vector<std::string>	&getSendBuffer(void);

		// *------ Attribute setters ------
		void						setSocketFd(int fd);
		void						setClientId(int id);
		void						setUsername(std::string username);
		void						setNickname(std::string nickname);
		void						setPass(bool set);
		void						setRegistered(bool set);

		// *------ Other functions ------
		void						fillInfo(int fd, std::string username, std::string nickname);
		void						postInfo(void);
};
