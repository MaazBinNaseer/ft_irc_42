/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 17:13:19 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/02 18:04:51 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <iomanip>
#include <csignal>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define RESET "\x1B[0m"

# define DEFAULT	"\033[0m"
# define PURPLE		"\033[0;35m"
# define CYAN		"\033[1;36m"

#define MINPORT 1024
#define MAXPORT 65535

#define BUFFER_SIZE 2048

#define CHANLIMIT 70    // max number of channels a client can join
#define NICKLEN 16      // max length of nickname
#define CHANNELEN 16    // max length of channel name
#define USERLEN 16      // max length of username
#define KICKLEN 255     // max length of KICK reason

#define OPERPASS "heeheeMJ"

class Channel;
class Client;
class Commands;
class Server;

#include "Channel.hpp"
#include "Client.hpp"
#include "Codes.hpp"
#include "Parse.hpp"
#include "Commands.hpp"
#include "Courier.hpp"
#include "Server.hpp"
