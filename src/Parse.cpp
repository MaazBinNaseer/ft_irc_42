#include "../inc/ft_irc.hpp"

Parse::Parse()
{
	this->_req_client = NULL;
	this->_serv = NULL;
}

Parse::Parse(Client *req_client, Server *srvptr): _req_client(req_client), _serv(srvptr)
{

}

Parse::~Parse()
{

}

std::string	Parse::getCmd()
{
	return (this->_cmd);
}

std::string	Parse::getCmdArg(unsigned long i)
{
	if (i < this->_cmd_args.size())
		return (this->_cmd_args[i]);
	return ("");
}

void	Parse::printCmdArgs()
{
	std::vector<std::string>::iterator it = this->_cmd_args.begin();
	std::vector<std::string>::iterator end = this->_cmd_args.end();

	std::cout << "ARGUMENTS: ";
	while (it < end)
		std::cout << *(it++) << " AND ";
	std::cout << std::endl;
	this->_cmd_args.clear();
}

int	Parse::getClientFd()
{
	return (this->_req_client->getSocketFd());
}

Client	*Parse::getReqClient()
{
	return (this->_req_client);
}

void	Parse::printClientData(Client *reqclient)
{
	reqclient->postInfo();
}

void	Parse::trim(std::string &buffer)
{
	std::string::size_type	new_line;
	std::string::size_type	white_space;

	white_space = buffer.find_first_not_of(" ");
	if (white_space != 0)
		buffer.erase(0, white_space); // ! Beginning spaces erased
	new_line = buffer.find_first_of("\n");
	white_space = new_line;
	white_space--;
	while (white_space != 0 && buffer[white_space] == ' ')
		white_space--;
	white_space++;
	if (white_space != 0 && white_space != new_line)
		buffer.erase(white_space, new_line - white_space); // ! Ending spaces erased
}

void	Parse::assignCommand(std::string &buffer)
{
	std::stringstream	read(buffer);
	std::string			command;
	unsigned long		new_line;
	unsigned long		next_word;

	getline(read, command, ' ');
	new_line = command.find('\n');
	if (new_line != std::string::npos)
		command.erase(new_line, 1); // ! newline removed from FIRST command
	this->_cmd = command;
	next_word = buffer.find_first_not_of(' ', buffer.find_first_of(' ')); // ! finds the next word after spaces
	buffer.erase(0, next_word); // ! first word and spaces after are erased
}

void	Parse::assignArguments(std::string &buffer)
{
	unsigned long		new_line;
	unsigned long		next_word;

	new_line = buffer.find('\n');
	if (new_line != std::string::npos)
		buffer.erase(new_line, 1); // ! newline erased
	std::stringstream	read(buffer);
	std::string			argument;
	while(!buffer.empty())
	{
		getline(read, argument, ' ');
		this->_cmd_args.push_back(argument);
		next_word = buffer.find_first_not_of(' ', buffer.find_first_of(' ')); // ! finds the next word after spaces
		buffer.erase(0, next_word); // ! first word and spaces after are erased
	}
}
