/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:46:17 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/08 15:47:47 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

void signalHandler(int signum)
{
	if (signum == SIGINT)
		std::cout << "SIGINT signal used" << std::endl;
}

int main(int argc, char *argv[])
{
	if (argc != 3 || !argv)
		return (std::cerr << RED "Enter in the format: ./ircserv <port> <pass>" RESET "\n", 1);

	try
	{
		Server	irc;
		signal(SIGINT, signalHandler);
		irc.bootup(argv[1], argv[2]);
	}
	catch(std::exception &e)
	{
		std::cerr << RED "Server stopped due to: " << e.what() << RESET << std::endl;
		return (1);
	}
	return (0);
}
