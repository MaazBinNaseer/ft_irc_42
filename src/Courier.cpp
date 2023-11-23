#include "../inc/ft_irc.hpp"

FailedFunction::FailedFunction(std::string function)
{
	std::string append;
	append = PURPLE + function + " Failed! \n" + RESET;
	this->_msg = append.c_str();
	std::cerr << this->_msg;
}

const char * FailedFunction::what() const throw()
{
	return ("Function Failure");
}

const char * SignalTrigger::what() const throw()
{
	return ("Signal");
}

CommandError::CommandError(const char *type, std::string code, std::string message, Client &client)
{
	type_of_error = type;
	serverMessage(code, RED + message + RESET, client);
}

const char * CommandError::what() const throw()
{
	return (type_of_error);
}

// *----- Log Commands -----

void logStart()
{
	std::ofstream log("Serverlog.txt", std::ios::app);
	log << DASH;
	log << "------------****************------------\n";
	log << "-----------******************-----------\n";
	log << "----------**SERVER LOG START**----------\n";
	log << "-----------******************-----------\n";
	log << "------------****************------------\n";
	log << DASH;
	log.close();
}

void logConnect(int fd)
{
	std::ofstream log("Serverlog.txt", std::ios::app);
	log << CORNER_UP;
	log << "** A client connected to the server, their socket fd is: " << fd << " **\n";
	log << CORNER_DOWN;
	log.close();
}

void logDisconnect(std::string reason, int fd)
{
	std::ofstream log("Serverlog.txt", std::ios::app);
	log << CORNER_UP;
	log << "** A client disconnected from the server, their socket fd is " << fd << " **\n";
	log << "Left because: " << reason << std::endl;
	log << CORNER_DOWN;
	log.close();
}

void logRecv(std::string input, int fd)
{
	std::ofstream log("Serverlog.txt", std::ios::app);
	log << CORNER_UP;
	log << "<<<RECEIVED<<< (from " << fd << "): \n" + input;
	log << CORNER_DOWN;
	log.close();
}

void logSend(std::deque<std::string> messages, int fd)
{
	std::ofstream log("Serverlog.txt", std::ios::app);
	log << CORNER_UP;
	log << ">>>SENDING>>> (to " << fd << "): \n";
	while(!messages.empty())
	{
		log << messages.front() << S;
		messages.pop_front();
	}
	log << std::endl;
	log << CORNER_DOWN;
	log.close();
}

void logRegister(Client &client)
{
	std::ofstream log("Serverlog.txt", std::ios::app);
	log << CORNER_UP;
	log << "Client " << client.getSocketFd() << " registered successfully" << std::endl;
	log << "Hello, my name is: " << client.getUsername() << std::endl;
	log << "with my hostname of: " << client.getHostname() << std::endl;
	log << "and my Nick name is: " << client.getNickname() << std::endl;
	log << "and my Real name is: " << client.getRealname() << std::endl;
	log << CORNER_DOWN;
	log.close();
}

void logEnd()
{
	std::ofstream log("Serverlog.txt", std::ios::app);
	log << DASH;
	log << "------------****************------------\n";
	log << "-----------******************-----------\n";
	log << "----------** SERVER LOG END **----------\n";
	log << "-----------******************-----------\n";
	log << "------------****************------------\n";
	log << DASH;
	log.close();
}

void serverLog(Client &acted, std::string target, std::string note)
{
	std::ofstream log("Serverlog.txt", std::ios::app);
	std::string message = "Client: " + TRIPLE_INFO(acted.getNickname(), acted.getUsername(), acted.getHostname()) + "\n";
	message += "Note: " + note + "\n";
	if (!target.empty())
		message += "Target: " + target + "\n";
	log << message;
	log.close();
}

// *----- Messages Sent to Clients -----

void broadcastTripleCommand(Client &recipient, Client &actor, std::string cmd, std::string msg)
{
	std::string message = TRIPLE_INFO(actor.getNickname(), actor.getUsername(), actor.getHostname());
	message += S + cmd;
	if (msg.at(0) != ':')
		msg = ":" + msg;
	message += S + msg + "\r\n";
	recipient.pushSendBuffer(message);
}

void broadcastallCommand(Client &recipient, Client &target, std::string cmd, std::string msg)
{
	std::string message = TRIPLE_INFO(target.getNickname(), target.getUsername(), target.getHostname());
	message += S + cmd;
	if (msg.at(0) != ':')
		msg = ":" + msg;
	message += S + msg + "\r\n";
	recipient.pushSendBuffer(message);
}

void customMessage(Client &recipient, std::string message)
{
	message += "\r\n";
	recipient.pushSendBuffer(message);
}

void selfCommand(Client &acted, std::string cmd, std::string msg)
{
	std::string message = TRIPLE_INFO(acted.getNickname(), acted.getUsername(), acted.getHostname());
	if (!cmd.empty())
		message += S + cmd;
	if (msg.at(0) != ':')
		msg = ":" + msg;
	message += S + msg + "\r\n";
	acted.pushSendBuffer(message);
}

void messageCommand(Client &acted, std::string target, std::string cmd, std::string msg)
{
	std::string message = TRIPLE_INFO(acted.getNickname(), acted.getUsername(), acted.getHostname());
	if (!cmd.empty())
		message += S + cmd;
	message += S + target;
	if (msg.at(0) != ':')
		msg = ":" + msg;
	message += S + msg + "\r\n";
	acted.pushSendBuffer(message);
}

void targettedCommand(Client &acted, Client &target, std::string cmd, std::string msg)
{
	std::string message = TRIPLE_INFO(acted.getNickname(), acted.getUsername(), acted.getHostname());
	message += S + cmd + S + target.getNickname();
	if (msg.at(0) != ':')
		msg = ":" + msg;
	message += S + msg + "\r\n";
	target.pushSendBuffer(message);
}

void serverMessage(std::string code, std::string message, Client &client)
{
	std::string send = ":" + client.getServername() + code + client.getNickname() + S +  ":" + PURPLE + message + RESET "\r\n";
	client.pushSendBuffer(send);
}

std::string messageOfTheDay()
{
	std::string message[] = {
		"You can't rely on how you look to sustain you, what sustains us, what is fundamentally beautiful is compassion; for yourself and your those around you.", 
		"Opportunity is missed by most people because it is dressed in overalls and looks like work."
		};
	int size = sizeof(message) / sizeof(message[0]);
    srand(time(NULL));  // Initialize the random seed once

    // Generate a random index and return the message
    int randomIndex = rand() % size;
    return (message[randomIndex]);
}


void welcomeMessage(Client &client, Server &server)
{
	std::string fdString = intToString(server.getFdSize() - 1);
	serverMessage(RPL_WELCOME, "Welcome to the Network, you are known as " + TRIPLE_INFO(client.getNickname(), client.getUsername(), client.getHostname()), client);
	serverMessage(RPL_YOURHOST, "Your host is Chirp_Association, running version 1.0", client);
	serverMessage(RPL_CREATED, "This server was created today", client);
	serverMessage(RPL_MYINFO, client.getServername() + " Chirp_Association version 1.0 N/A itkol", client);
	serverMessage(RPL_ISUPPORT, "SUPPORTED TOKENS:", client);
	std::string	msg = "CHANLIMIT=#:" + intToString(CHANLIMIT) + " NICKLEN=" + intToString(NICKLEN) + " CHANNELEN=" + intToString(CHANNELEN);
	msg += " USERLEN=" + intToString(USERLEN) + " KICKLEN=" + intToString(KICKLEN) + ": are supported by the server";
	serverMessage(RPL_ISUPPORT, msg, client);
	serverMessage(RPL_STATISTICS, "I have " + fdString  + " clients and 1 server", client);
	// ------------------------------------------------------------------------------------------------------------------------------
	std::string MOTD = GREEN "Message of the Day: " + messageOfTheDay();
	serverMessage(RPL_WELCOME, MOTD + RESET, client);
}

std::string intToString(int value) 
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

void capitalize(std::string &cmd)
{
	for (unsigned long i = 0; i < cmd.size(); i++)
	{
		if (cmd[i] >= 'a' && cmd[i] <= 'z')
			cmd[i] -= 32;	
	}
}
