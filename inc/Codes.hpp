#pragma once

//*--------------------------
//*----- Reply Messages -----
//*--------------------------
//* ===== Registeration =====
#define	RPL_WELCOME				" 001 "
#define	RPL_YOURHOST			" 002 "
#define	RPL_CREATED				" 003 "
#define	RPL_MYINFO				" 004 "
#define	RPL_ISUPPORT			" 005 "
//* ===== Statisitics =====
#define RPL_STATISTICS			" 055 "
//* ===== Client Mode =====
#define	RPL_UMODEIS				" 221 "
//* ===== WHOIS =====
#define	RPL_WHOISREGNICK		" 307 "
#define	RPL_WHOISUSER			" 311 "
#define	RPL_WHOISSERVER			" 312 "
#define	RPL_WHOISOPERATOR		" 313 "
#define	RPL_ENDOFWHOIS			" 318 "
//* ===== Channel Mode =====
#define	RPL_CHANNELMODEIS		" 324 "
//* ===== Channel Topic =====
#define	RPL_NOTOPIC				" 331 "
#define	RPL_TOPIC				" 332 "
//* ===== WHOIS =====
#define	RPL_WHOISACTUALLY		" 338 "
//* ===== INVITE =====
#define	RPL_INVITING			" 341 "
//* ===== MOTD =====
#define	RPL_MOTD				" 372 "
#define	RPL_MOTDSTART			" 375 "
#define	RPL_ENDOFMOTD			" 376 "
//* ===== WHOIS =====
#define	RPL_WHOISHOST			" 378 "
//* ===== OPER =====
#define	RPL_YOUREOPER			" 381 "

//*--------------------------
//*----- Error Messages -----
//*--------------------------
#define ERR_UNKNOWNERROR		" 400 "
//* ===== NICK =====
#define	ERR_NOSUCHNICK			" 401 "
//* ===== Channel =====
#define ERR_NOSUCHCHANNEL		" 403 "
#define	ERR_CANNOTSENDTOCHAN	" 404 "
#define	ERR_TOOMANYCHANNELS		" 405 "
//* ===== PRIVMSG =====
#define	ERR_NORECIPIENT			" 411 "
#define	ERR_NOTEXTOSEND			" 412 "
#define	ERR_INPUTTOOLONG		" 417 "
//* ===== Command =====
#define	ERR_UNKNOWNCOMMAND		" 421 "
//* ===== MOTD =====
#define	ERR_NOMOTD				" 422 "
//* ===== NICK =====
#define ERR_NONICKNAMEGIVEN		" 431 "
#define	ERR_NICKNAMEINUSE		" 433 "
//* ===== Channel =====
#define ERR_USERNOTINCHANNEL	" 441 "
#define	ERR_NOTONCHANNEL		" 442 "
#define ERR_USERONCHANNEL		" 443 "
//* ===== Registeration =====
#define ERR_NOTREGISTERED		" 451 "
//* ===== Parsing =====
#define ERR_NEEDMOREPARAMS		" 461 "
//* ===== Registeration =====
#define ERR_ALREADYREGISTERED	" 462 "
//* ===== PASS =====
#define ERR_PASSWDMISMATCH		" 464 "
//* ===== JOIN =====
#define ERR_CHANNELISFULL		" 471 "
//* ===== MODE =====
#define	ERR_UNKNOWNMODE			" 472 "
//* ===== JOIN =====
#define ERR_INVITEONLYCHAN		" 473 "
#define ERR_BANNEDFROMCHAN		" 474 "
#define ERR_BADCHANMASK			" 476 "
//* ===== IRC Operator =====
#define ERR_NOPRIVILEGES		" 481 "
//* ===== Channel Operator =====
#define ERR_CHANOPRIVSNEEDED	" 482 "
//* ===== KILL =====
#define ERR_CANTKILLSERVER		" 483 "
//* ===== MODE =====
#define ERR_UMODEUNKNOWNFLAG	" 501 "
#define ERR_USERSDONTMATCH		" 502 "