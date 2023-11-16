/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:46:17 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/16 19:16:39 by mgoltay          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

int main(int argc, char *argv[])
{
	if (argc != 3 || !argv)
		return (std::cerr << RED "Enter in the format: ./ircserv <port> <pass>" RESET "\n", 1);

	try
	{
		Server	irc;
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
