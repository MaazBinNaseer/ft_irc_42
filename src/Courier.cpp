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

void serverLog(Client &acted, std::string target, std::string cmd, std::string note) // ! incase of debugging
{
	std::string message = "Client: " + TRIPLE_INFO(acted.getNickname(), acted.getUsername(), acted.getHostname()) + "\n";
	message += "Command: " + cmd + "\n";
	if (!target.empty())
		message += "Target: " + target + "\n";
	message += "Note: " + note + "\n";
	std::cout << message << std::endl;
}

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
	serverMessage(RPL_MYINFO, client.getHostname() + "\r\n", client);
	serverMessage(RPL_ISUPPORT, " :SUPPORTED TOKENS\r\n", client); // ! we need to discuss these tokens
}
