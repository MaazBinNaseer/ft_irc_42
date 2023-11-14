/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:29:30 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/14 19:04:29 by amalbrei         ###   ########.fr       */
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
		std::string					_servername;
		std::string					_hostname;
		std::string					_username;
		std::string					_nickname;
		std::string					_realname;
		bool						_pass;
		bool						_registered;
		bool						_remove;
		std::string					_reason;
		std::string					_receiveBuffer;
		std::deque<std::string> 	_sendBuffer;
		t_cap						_caps;
		unsigned int				_factindex;
		bool						_triviaMode;
		unsigned int				_triviaindex;
		std::string					_triviaAnswer;

    public:
		bool						_cap_order;
	
        // *----- Orthodox Canonical Form -----
		t_cap getDefaultCap();
		Client();
		Client(unsigned int fd, std::string hostname);
		Client( const Client &f );
		Client & operator=(Client const & rhs);
        ~Client();

		// *------ Attribute getters ------
		int							getSocketFd(void) const ;
		std::string					getServername(void) const ;
		std::string					getUsername(void) const ;
		std::string					getHostname(void) const ;
		std::string					getNickname(void) const ;
		std::string					getRealname(void) const ;
		bool						getPass(void) const ;
		bool						getRegistered(void) const ;
		bool						getRemove(void) const ;
		std::string					getReason(void) const ;
		std::string					&getReceiveBuffer(void);
		std::deque<std::string>		&getSendBuffer(void);
		t_cap						getCaps(void) const ;
		unsigned int				getFactIndex(void) const;
		bool						getTriviaMode(void) const;
		unsigned int				getTriviaIndex(void) const;
		std::string					getTriviaAnswer(void) const;

		// *------ Attribute setters ------
		void						setSocketFd(int fd);
		void						setServername(std::string servername);
		void						setUsername(std::string username);
		void						setHostname(std::string nickname);
		void						setNickname(std::string nickname);
		void						setRealname(std::string nickname);
		void						setPass(bool set);
		void						setRegistered(bool set);
		void						setRemove(bool set);
		void						setReason(std::string reason);
		void						pushSendBuffer(std::string string);
		void						setCaps(unsigned long i, bool state);
		void						setFactIndex(unsigned int index);
		void						setTriviaMode(bool status);
		void						setTriviaIndex(unsigned int index);
		void						setTriviaAnswer(std::string answer);
	
		// *------ Other functions ------
		void						appendExecBuffer(std::string newbuff, Server *_serv);
		void						sendmsg(std::string msg);
		void						print(std::string color);
};
