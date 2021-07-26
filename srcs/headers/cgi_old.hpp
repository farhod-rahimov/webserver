#ifndef WEBSERV_CGI_H
# define WEBSERV_CGI_H

// ====== COLORS ======
# define BLACK "\x1B[30m"
# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define BLUE "\x1B[34m"
# define MAGENTA "\x1B[35m"
# define CYAN "\x1B[36m"
# define WHITE "\x1B[37m"
# define RESET "\x1B[0m"
// ====================

# define IN 0
# define OUT 1
# define CGI_BUFSIZE 65535
# define BODY_SEP "\r\n\r\n"
# define CRLF "\r\n"

# include "Header.hpp"
# include "Client.hpp"
# include "Server.hpp"

// class Client;
// class Server;



class CGI {
public:

	CGI();
	virtual ~CGI();

	char **getEnvironment() const;
	char **setEnvToString(std::map<std::string, std::string> env) ;
	void executeCGI();
	void clean();
	void init(Server *server, Client *client, const char *path);
	bool isInit();
	void queryStringParse(Client *client, std::string path);
	void setEnvironment();

private:

	bool _init;
	CGI &operator=(const CGI &other);
	char **_environment;
	char **_arguments;
	char *_path;

	std::string _queryString;

	Client *_client;
	Server *_server;

	std::string _clientHeader;
	
	long 			_bodySize;
	CGI(const CGI &other);

};


#endif //WEBSERV_CGI_H
