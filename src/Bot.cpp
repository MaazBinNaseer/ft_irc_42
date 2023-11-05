#include "../inc/ft_irc.hpp"


Bot:: Bot(void)
{}

Bot::~Bot(void)
{}

void Bot::factBot(Client *target)
{
    srand(static_cast<unsigned int>(time(NULL)));

    target->sendmsg("I am the Fact Bot. My job is to give facts at every 30 seconds interval\n ");
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