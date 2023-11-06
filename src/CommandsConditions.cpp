#include "../inc/ft_irc.hpp"

void Commands::checkPassword(char flag)
{
	switch (flag)
	{
		case CHECK:
		{
			if (this->_serv->getPassword() != getCmdArg(0))
				throw CommandError("Password Incorrect", ERR_PASSWDMISMATCH, "Password incorrect", *_req_client);
			break ;
		}
		case REQUIRED:
		{
			if (!this->_req_client->getPass())
				throw CommandError("Password Required", ERR_PASSWDMISMATCH, "Password needed", *_req_client);
			break ;
		}
		case OPERATOR:
		{
			if (getCmdArg(1) != this->_serv->getOperPass())
				throw CommandError("Password Incorrect", ERR_PASSWDMISMATCH, "Incorrect Password for Operator Privileges!", *_req_client);
			break ;
		}
		default:
			std::cout << RED "Used wrong flag or last case follows through" RESET << std::endl;
	}
}

void Commands::checkNickname(char flag)
{
	Client	*targetcl = this->_serv->getClientNick(getCmdArg(0));
	Client	*targetcl2 = this->_serv->getClientNick(getCmdArg(1));
	switch (flag)
	{
		case NONICK:
		{
			if (_req_client->getNickname() == "*")
				throw CommandError("No Nickname Given", ERR_NONICKNAMEGIVEN, "Nickname required to register using NICK <nickname>", *_req_client);
			break ;
		}
		case NOFOUND0:
		{
			if (!targetcl)
				throw CommandError("Nickname Not Found", ERR_NOSUCHNICK, "User does not Exist!", *_req_client);
			break ;
		}
		case NOFOUND1:
		{
			if (!targetcl2)
				throw CommandError("Nickname Not Found", ERR_NOSUCHNICK, "User does not Exist!", *_req_client);
			break ;
		}
		case INUSEN:
		{
			if (targetcl)
				throw CommandError("Nickname In Use", ERR_NICKNAMEINUSE, getCmdArg(0) + " Nickname is already in use by another user", *_req_client);
			break ;
		}
		case INUSECH:
		{
			if (this->_serv->getChannel(getCmdArg(0)))
				throw CommandError("Nickname In Use", ERR_NICKNAMEINUSE, getCmdArg(0) + " Nickname is already in use by a channel", *_req_client);
			break ;
		}
		case OPERATORN:
		{
			if (!this->_serv->isOp(*this->_req_client))
				throw CommandError("Operator Privilege Required", ERR_NOPRIVILEGES, "Permission Denied - You're not an IRC operator", *_req_client);
			break ;
		}
		default:
			std::cout << RED "Used wrong flag or last case follows through" RESET << std::endl;
	}
}

void Commands::checkLen(char flag)
{
	switch (flag)
	{
		case NICKNAMEL:
		{
			if (getCmdArg(0).size() > NICKLEN)
				throw CommandError("Nickname Too Long", ERR_INPUTTOOLONG, getCmdArg(0) + " Nickname is too long", *_req_client);
			break ;
		}
		case USERNAMEL:
		{
			if (getCmdArg(0).size() > USERLEN)
				throw CommandError("Username Too Long", ERR_INPUTTOOLONG, getCmdArg(0) + " Username is too long", *_req_client);
			break ;
		}
		case CHANNELL:
		{
			if (getCmdArg(0).size() > CHANNELEN)
				throw CommandError("Channel Name Too Long", ERR_INPUTTOOLONG, getCmdArg(0) + " Channel Name is too long", *_req_client);
			break ;
		}
		default:
			std::cout << RED "Used wrong flag or last case follows through" RESET << std::endl;
	}
}

void Commands::checkChannel(char flag)
{
	Channel *targetch = this->_serv->getChannel(getCmdArg(0));
	switch (flag)
	{
		case EXIST:
		{
			if (!targetch)
				throw CommandError("Channel Doesn't Exist", ERR_NOSUCHCHANNEL, getCmdArg(0) + " No such channel", *_req_client);
			break ;
		}
		case INVITEO:
		{
			if (targetch && targetch->isInviteOnly())
				throw CommandError("Invite Only", ERR_INVITEONLYCHAN, targetch->getName() + " Cannot join channel (+i)", *_req_client);
			break ;
		}
		case PASSWORDC:
		{
			if (targetch && targetch->getPassword() != "" && targetch->getPassword() != getCmdArg(1))
				throw CommandError("Wrong Password", ERR_PASSWDMISMATCH, getCmdArg(1) + " Password Incorrect", *_req_client);
			break ;
		}
		case LIMIT:
		{
			if (this->_serv->getChannels(this->_req_client).size() >= CHANLIMIT)
				throw CommandError("Channel Limit Reached", ERR_TOOMANYCHANNELS, targetch->getName() + " Channel Limit Reached", *_req_client);
			break ;
		}
		case NOTPART:
		{
			if (targetch && !targetch->exists(*_req_client))
				throw CommandError("Channel Incompatibility", ERR_NOTONCHANNEL, targetch->getName() + " You're not on that channel", *_req_client);
			break ;
		}
		case OPERATORC:
		{
			if (targetch && !targetch->isOp(*_req_client))
				throw CommandError("Operator Privilege Required", ERR_CHANOPRIVSNEEDED, targetch->getName() + " You're not the channel operator", *_req_client);
			break ;
		}
		default:
			std::cout << RED "Used wrong flag or last case follows through" RESET << std::endl;
	}
}

void Commands::checkConditions(std::string flags)
{
	for (unsigned long i = 0; i < flags.size(); i++)
	{
		switch (flags[i])
		{
			case _CHANNEL:
			{
				checkChannel(flags[++i]);
				break ;
			}
			case _LEN:
			{
				checkLen(flags[++i]);
				break ;
			}
			case _PARAM:
			{
				unsigned long num = flags[++i] - '0';
				if (this->_cmd_args.size() < num)
					throw CommandError("Insufficient Parameters", ERR_NEEDMOREPARAMS, this->_cmd + " Not enough parameters, you need " + flags[i], *_req_client);
				break ;
			}
			case _NICKNAME:
			{
				checkNickname(flags[++i]);
				break ;
			}
			case _PASSWORD:
			{
				checkPassword(flags[++i]);
				break ;
			}
			case _REGIS:
			{
				if (_req_client->getRegistered())
					throw CommandError("Already Registered", ERR_ALREADYREGISTERED, "No need to reregister", *_req_client);
				break ;
			}
			default:
				std::cout << RED "Used wrong flag or last case follows through" RESET << std::endl;
		}
	}
}