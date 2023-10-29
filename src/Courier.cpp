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

NeedMoreParams::NeedMoreParams(std::string params): _no_of_params(params.c_str())
{

}

const char * NeedMoreParams::what() const throw()
{
	return (_no_of_params);
}

// *----- Log Commands -----

void logStart(std::ofstream &log)
{
	log.open("Serverlog.txt", std::ios::app);
	log << DASH;
	log << "------------****************------------\n";
	log << "-----------******************-----------\n";
	log << "----------**SERVER LOG START**----------\n";
	log << "-----------******************-----------\n";
	log << "------------****************------------\n";
	log << DASH;
	log.close();
}

void logConnect(int fd, std::ofstream &log)
{
	log.open("Serverlog.txt", std::ios::app);
	log << CORNER_UP;
	log << "** A client connected to the server, their socket fd is: " << fd << " **\n";
	log << CORNER_DOWN;
	log.close();
}

void logDisconnect(std::string reason, int fd, std::ofstream &log)
{
	log.open("Serverlog.txt", std::ios::app);
	log << CORNER_UP;
	log << "** A client disconnected from the server, their socket fd is " << fd << " **\n";
	log << "Left because: " << reason << std::endl;
	log << CORNER_DOWN;
	log.close();
}

void logRecv(std::string input, int fd, std::ofstream &log)
{
	log.open("Serverlog.txt", std::ios::app);
	log << CORNER_UP;
	log << "<<<RECEIVED<<< (from " << fd << "): \n" + input;
	log << CORNER_DOWN;
	log.close();
}

void logSend(std::deque<std::string> messages, int fd, std::ofstream &log)
{
	log.open("Serverlog.txt", std::ios::app);
	log << CORNER_UP;
	log << ">>>SENDING>>> (from " << fd << "): \n";
	while(!messages.empty())
	{
		log << messages.front() << " ";
		messages.pop_front();
	}
	log << std::endl;
	log << CORNER_DOWN;
	log.close();
}

void logEnd(std::ofstream &log)
{
	log.open("Serverlog.txt", std::ios::app);
	log << DASH;
	log << "------------****************------------\n";
	log << "-----------******************-----------\n";
	log << "----------** SERVER LOG END **----------\n";
	log << "-----------******************-----------\n";
	log << "------------****************------------\n";
	log << DASH;
	log.close();
}

void serverLog(Client &acted, std::string target, std::string cmd, std::string note) // ! incase of debugging
{
	std::string message = "Client: " + TRIPLE_INFO(acted.getNickname(), acted.getUsername(), acted.getHostname()) + "\n";
	message += "Command: " + cmd + "\n";
	if (!target.empty())
		message += "Target: " + target + "\n";
	message += "Note: " + note + "\n";
	std::cout << message << std::endl;
}

// *----- Messages Sent to Clients -----

void selfCommand(Client &acted, std::string cmd, std::string msg)
{
	std::string message = ":" + TRIPLE_INFO(acted.getNickname(), acted.getUsername(), acted.getHostname());
	message += S + cmd + S;
	message += ":" + msg;
	acted.pushSendBuffer(message);
}

void targettedCommand(Client &acted, Client &target, std::string cmd, std::string msg)
{
	std::string message = ":" + TRIPLE_INFO(acted.getNickname(), acted.getUsername(), acted.getHostname());
	message += S + cmd + S + target.getNickname();
	message += ":" + msg;
	target.pushSendBuffer(message);
}

void serverMessage(std::string code, std::string message, Client &client)
{
	std::string send = ":" + client.getServername() + code + client.getNickname() + S + message;
	client.pushSendBuffer(send);
}

void welcomeMessage(Client &client)
{
	serverMessage(RPL_WELCOME, " :Welcome to the Network, you are known as " + TRIPLE_INFO(client.getNickname(), client.getUsername(), client.getHostname()) + "\r\n", client);
	serverMessage(RPL_YOURHOST, " :Your host is " + client.getHostname() + "\r\n", client);
	serverMessage(RPL_CREATED, " :This server was created today\r\n", client);
	serverMessage(RPL_MYINFO, " :" + client.getHostname() + "\r\n", client);
	serverMessage(RPL_ISUPPORT, " :SUPPORTED TOKENS\r\n", client); // ! we need to discuss these tokens
}
