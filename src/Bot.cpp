#include "../inc/ft_irc.hpp"


Bot:: Bot(void)
{
}

Bot::~Bot(void)
{}

void Bot::intializeTrivia()
{
    // if (!_triviaStart)
    //     return;
    _triviaQuestions.push_back("What is the capital of France?");
    std::vector<std::string> options;
    options.push_back("A) London");
    options.push_back("B) Berlin");
    options.push_back("C) Paris");
    options.push_back("D) Madrid");
    _triviaOptions.push_back(options);

    _triviaAnswers.push_back('C');

    _triviaQuestions.push_back("What is the capital of UAE?");
    std::vector<std::string> options1;
    options1.push_back("A) Dubai");
    options1.push_back("B) Sharjah");
    options1.push_back("C) Ruwais");
    options1.push_back("D) Abu Dhabi");
    _triviaOptions.push_back(options1);
    _triviaAnswers.push_back('D');

    _triviaStart = true;
}

void Bot::triviaBot(Client *target)
{
    target->sendmsg(GREEN "Welcome to triviia Bot\n" RESET);
    
    intializeTrivia();
    int questionIndex = rand() % _triviaQuestions.size();
    std::string question = _triviaQuestions[questionIndex];
    std::vector<std::string> options = _triviaOptions[questionIndex];
    // char correctAnswer = _triviaAnswers[questionIndex];

    target->sendmsg("Trivia Time! Answer the following question:\n");
        target->sendmsg(question + "\n");
        for (size_t i = 0; i < options.size(); ++i) {
            target->sendmsg(options[i] + "\n");
        }   
        target->sendmsg("Please respond with the correct letter (e.g., 'A').\n");
}

void Bot::factBot(Client *target)
{
    srand(static_cast<unsigned int>(time(NULL)));

    target->sendmsg(YELLOW "I am the Fact Bot. My job is to give facts at every 30 seconds interval\n " RESET);
    for(size_t i = 0; i < 10; ++i)
        {
            std::string info[] = {
            "Fact 1: The heart of a shrimp is located in its head.",
            "Fact 2: A snail can sleep for three years.",
            "Fact 3: The fingerprints of a koala are virtually indistinguishable from those of humans.",
            "Fact 4: Slugs have four noses.",
            "Fact 5: Bananas are curved because they grow towards the sun.",
            "Fact 6: A duck's quack doesn't echo, and no one knows why.",
            "Fact 7: An ostrich's eye is bigger than its brain.",
            "Fact 8: Most lipstick contains fish scales.",
            "Fact 9: Like fingerprints, everyone's tongue print is different.",
            "Fact 10: An octopus has three hearts."
        };
        int factIndex = rand() % 10;
        target->sendmsg(info[factIndex] + "\n");
        sleep(20);
    }
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

    target_client->sendmsg(botArt);
    target_client->sendmsg(YELLOW "Welcome to the bot server\n" RESET);
    target_client->sendmsg(GREEN "Please select the bot of you choicde.\n 1.Fact Bot\n 2.Trivia Bot\n" RESET);
}

 std::string Bot::getCmdArg(unsigned long i)
 {
    if (i < this->_cmd_args.size())
		return (this->_cmd_args[i]);
	return ("");
 }