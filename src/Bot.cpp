/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amalbrei <amalbrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 16:15:11 by amalbrei          #+#    #+#             */
/*   Updated: 2023/11/18 19:10:20 by amalbrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_irc.hpp"

Bot::Bot(void)
{

}

Bot::~Bot(void)
{

}

void Bot::intializeTrivia(unsigned int index)
{
	if (index == 0)
	{
		_triviaQuestion.push_back("What is the capital of France?");
		
		_triviaOptions.push_back("A) London");
		_triviaOptions.push_back("B) Berlin");
		_triviaOptions.push_back("C) Paris");
		_triviaOptions.push_back("D) Madrid");
		 
		_triviaAnswer.push_back("C");
	}
	else if (index == 1)
	{
    	_triviaQuestion.push_back("What is the capital of UAE?");
		
    	_triviaOptions.push_back("A) Dubai");
    	_triviaOptions.push_back("B) Sharjah");
    	_triviaOptions.push_back("C) Ruwais");
    	_triviaOptions.push_back("D) Abu Dhabi");
		
    	_triviaAnswer.push_back("D");
	}
}

void Bot::triviaBot(Client *target, std::string answer)
{
	if (!target->getTriviaMode())
	{
		selfCommand(*target, "PRIVMSG #bot ", GREEN "Welcome to Trivia Bot\n" RESET);
    	intializeTrivia(target->getTriviaIndex());
		target->setTriviaIndex(target->getTriviaIndex() + 1);
		selfCommand(*target, "PRIVMSG #bot ", YELLOW "Trivia Time! Answer the following question:" RESET);
		selfCommand(*target, "PRIVMSG #bot ", YELLOW + this->_triviaQuestion.front() + RESET);
		this->_triviaQuestion.clear();
		for (std::vector<std::string>::iterator it = this->_triviaOptions.begin(); it != this->_triviaOptions.end(); it++)
			selfCommand(*target, "PRIVMSG #bot ", YELLOW + *it + RESET);
		this->_triviaOptions.clear();
		target->setTriviaAnswer(this->_triviaAnswer.front());
		this->_triviaAnswer.clear();
		selfCommand(*target, "PRIVMSG #bot ", YELLOW "Please respond with the correct letter" GREEN " (e.g: PRIVMSG #bot 2 <answer(as a letter)>)" RESET);
		target->setTriviaMode(true);
	}
	else if (target->getTriviaMode())
	{
		capitalize(answer);
		if (answer.size() == 1)
		{
			if (answer == target->getTriviaAnswer())
				selfCommand(*target, "PRIVMSG #bot ", GREEN "CORRECT Answer! Well done (y)" RESET);
			else
				selfCommand(*target, "PRIVMSG #bot ", CYAN "Wrong Answer.... Better luck next time :)" RESET);
			target->setTriviaMode(false);
		}
		else
			selfCommand(*target, "PRIVMSG #bot ", RED "Answer sent in wrong format, only type in a single letter!" RESET);
	}
}

void Bot::factBot(Client *target)
{
	selfCommand(*target, "PRIVMSG #bot ", GREEN "Here is a random fact" RESET);
	std::string info[] = {
    "The heart of a shrimp is located in its head.",
    "A snail can sleep for three years.",
    "The fingerprints of a koala are virtually indistinguishable from those of humans.",
    "Slugs have four noses.",
    "Bananas are curved because they grow towards the sun.",
    "A duck's quack doesn't echo, and no one knows why.",
    "An ostrich's eye is bigger than its brain.",
    "Most lipstick contains fish scales.",
    "Like fingerprints, everyone's tongue print is different.",
    "An octopus has three hearts.",
	"Studies have shown that cows become stressed when separated from their best friends, indicating complex emotional relationships.",
    "A Group of Crows is Called a Murder: This term for a group of crows dates back to medieval times and is thought to derive from the ominous associations with crows and death.",
    "Humans Share 50% of Their DNA with Bananas: This surprising genetic similarity is a testament to the common ancestry of all living organisms.",
    "The World's Oldest Known “Your Mom” Joke is 3,500 Years Old: Discovered in a Babylonian tablet from 1500 B.C., it is believed to be the oldest surviving joke of its kind.",
    "The Moon Has Moonquakes: Similar to earthquakes on Earth, the moon experiences moonquakes that are caused by tidal stresses, meteorite impacts, and thermal expansion.",
    "Chickens Can Remember Over 100 Faces: Chickens have impressive memory and recognition skills, allowing them to remember and distinguish between over 100 different animals and humans."
    };
	selfCommand(*target, "PRIVMSG #bot ", YELLOW + info[target->getFactIndex()] + RESET);
	target->setFactIndex(target->getFactIndex() + 1);
}


void Bot::BotIntroduction(Client *target_client)
{
    std::string botArt = 
    "      ___       ___\n"
    "     [___] /~  [___]\n"
    "     |ooo|._/_.|ooo|\n"
    "     |888||   ||888|\n"
    "    /|888||   ||888|\\\n"
    "  /_,|###||___||###|._\\\n"
    " /~\\  ~~~ /[_]\\ ~~~  /~\\\n"
    "(O_O) /~~[_____]~~\\ (O_O)\n"
    "     (  |       |  )\n"
    "    [~` ]       [ '~]\n"
    "    |~~|         |~~|\n"
    "    |  |         |  |\n"
    "   _<\\/>_       _<\\/>_ \n"
    "  /_====_\\     /_====_\\\n";

	while (!botArt.empty())
	{
		selfCommand(*target_client, "PRIVMSG #bot ", botArt.substr(0, botArt.find('\n')));
		botArt.erase(0, botArt.find('\n') + 1);
	}
    selfCommand(*target_client, "PRIVMSG #bot ", YELLOW "Welcome to the bot server" RESET);
    selfCommand(*target_client, "PRIVMSG #bot ", GREEN "Please select the bot of you choice" RESET);
    selfCommand(*target_client, "PRIVMSG #bot ", GREEN "1.Fact Bot" RESET);
    selfCommand(*target_client, "PRIVMSG #bot ", GREEN "2.Trivia Bot" RESET);
}
