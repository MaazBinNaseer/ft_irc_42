#ifndef BOT_HPP
#define BOT_HPP

#include "ft_irc.hpp"
#include "Parse.hpp"

class Bot: public Parse
{
    private:
        bool                                    _triviaStart;
        std::vector<std::string>                _triviaQuestions;
        std::vector<std::vector<std::string> >  _triviaOptions;
        std::vector<char>                       _triviaAnswers;

    public:
        Bot(void);
        ~Bot(void);
        void BotIntroduction(Client *target);
        std::string getCmdArg(unsigned long i);
        void factBot(Client *target);
        void intializeTrivia();
        void triviaBot(Client *target);
        // int getOption();
};



#endif