/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgoltay <mgoltay@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/15 16:46:17 by mgoltay           #+#    #+#             */
/*   Updated: 2023/11/03 16:51:14 by mgoltay          ###   ########.fr       */
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
		irc.bootup(argv[1], argv[2]);
	}
	catch(std::exception &e)
	{
		std::cerr << RED "Server stopped due to: " << e.what() << RESET << std::endl;
		return (1);
	}
	return (0);
}
