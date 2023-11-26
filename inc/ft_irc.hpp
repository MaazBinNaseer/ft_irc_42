/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 17:13:19 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/26 12:43:38 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <csignal>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>

#define GREY	"\x1B[30m"
#define RED		"\x1B[31m"
#define GREEN	"\x1B[32m"
#define YELLOW	"\x1B[33m"
#define BLUE	"\x1B[34m"
#define PURPLE	"\x1B[35m"
#define CYAN	"\x1B[36m"
#define RESET	"\x1B[0m"

#define MINPORT 1024
#define MAXPORT 65535

#define DEBUG 3
// 0 - SUBMISSION STATE
// 1 - DISPLAY INPUTS ONLY
// 2 - DISPLAY OUTPUTS ONLY
// 3 - DISPLAY ALL

#define BUFFER_SIZE 2048

#define CHANLIMIT 70    // max number of channels a client can join
#define NICKLEN 16      // max length of nickname
#define CHANNELEN 16    // max length of channel name
#define USERLEN 16      // max length of username
#define KICKLEN 255     // max length of KICK reason

#define OPERPASS "heeheeMJ"

class Channel;
class Bot;
class Client;
class Commands;
class Server;

#include "Channel.hpp"
#include "Bot.hpp"
#include "Client.hpp"
#include "Codes.hpp"
#include "Commands.hpp"
#include "Courier.hpp"
#include "Server.hpp"
