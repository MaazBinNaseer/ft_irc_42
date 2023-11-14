/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:10:01 by amalbrei          #+#    #+#             */
/*   Updated: 2023/11/14 19:02:25 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Bot
{
    private:
        bool						_triviaStart;
        std::vector<std::string>	_triviaQuestion;
        std::vector<std::string> 	_triviaOptions;
        std::vector<std::string>	_triviaAnswer;

    public:
        Bot(void);
        ~Bot(void);
        void BotIntroduction(Client *target);
        std::string getCmdArg(unsigned long i);
        void factBot(Client *target);
		void intializeTrivia(unsigned int index);
        void triviaBot(Client *target, std::string answer);
};
