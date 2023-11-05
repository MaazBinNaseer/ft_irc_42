#ifndef BOT_HPP
#define BOT_HPP

#include "ft_irc.hpp"
#include "Parse.hpp"

class Bot: public Parse
{
    private:
        std::vector<std::string> _facts;
        std::vector<std::string> _trivia;

    public:
        Bot(void);
        ~Bot(void);
        void BotIntroduction(Client *target);
        std::string getCmdArg(unsigned long i);
        void factBot(Client *target);
        // int getOption();
};



#endif