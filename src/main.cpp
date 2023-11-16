/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:46:17 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/16 17:49:29 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"
#include <sys/signal.h>

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{}
	if (signum == SIGTSTP)
	{}
	if (signum == SIGQUIT)
	{}
}

int main(int argc, char *argv[])
{
	if (argc != 3 || !argv)
		return (std::cerr << RED "Enter in the format: ./ircserv <port> <pass>" RESET "\n", 1);

	try
	{
		Server	irc;
		signal(SIGINT, signalHandler);
		signal(SIGTSTP, signalHandler);
		signal(SIGQUIT, signalHandler);
		return (irc.bootup(argv[1], argv[2]));
	}
	catch(std::exception &e)
	{
		std::string exit = e.what();
		if (exit == "Signal")
			return (std::cout << CYAN "Signal Terminated! Goodbye!" RESET "\n", 0);
		return (std::cerr << RED "Server stopped due to: " << e.what() << RESET << std::endl, 1);
	}
}
