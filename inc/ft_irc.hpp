/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 17:13:19 by mgoltay           #+#    #+#             */
/*   Updated: 2023/10/23 16:02:07 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <ostream>
#include <sstream>
#include <cstring>
#include <stdbool.h>
#include <string>
#include <vector>
#include <map>

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

#define OPERPASS "heeheeMJ"

class Server; // * need to include so other classes can use Server as attribute

#include "Client.hpp"
#include "Channel.hpp"
#include "Parse.hpp"
#include "Commands.hpp"
#include "Server.hpp"
