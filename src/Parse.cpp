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
	std::cout << "ARGUMENTS: ";
	for (size_t i = 0; i < this->_cmd_args.size(); i++)
		std::cout << "[" << i << "] " << this->_cmd_args[i] << " AND ";
	std::cout << std::endl;
}

int	Parse::getClientFd()
{
	return (this->_req_client->getSocketFd());
}

Client	*Parse::getReqClient()
{
	return (this->_req_client);
}

void	Parse::trim(std::string &buffer)
{
	while (buffer[0] == ' ')
		buffer.erase(0, 1);
	for (unsigned long i = 0; i < buffer.size() - 1; i++)
	{
		if (buffer[i] == ' ' && buffer[i + 1] == ' ')
			buffer.erase(i--, 1);
		else if (buffer[i] == '"' && ++i)
			while (i < buffer.size() && buffer[i] != '"')
				i++;
		else if (buffer[i] == '\'' && ++i)
			while (i < buffer.size() && buffer[i] != '\'')
				i++;
	}
	while (buffer.size() > 1 && (buffer[buffer.size() - 2] == ' ' || buffer[buffer.size() - 2] == '\r'))
		buffer.erase(buffer.size() - 2, 1);
	buffer.erase(buffer.size() - 1, 1);
}

std::string	Parse::extractWord(std::string &buffer)
{
	size_t	pos = 0;
	while (pos < buffer.size())
	{
		if (buffer[pos] == '"' && ++pos)
			while (pos < buffer.size() && buffer[pos] != '"')
				pos++;
		else if (buffer[pos] == '\'' && ++pos)
			while (pos < buffer.size() && buffer[pos] != '\'')
				pos++;
		else if (buffer[pos] == ' ')
			break ;
		pos++;
	}
	while (buffer[pos] == ' ')
		buffer.erase(pos, 1);
	std::string ret = buffer.substr(0, pos);
	buffer.erase(0, pos);
	return (ret);
}
