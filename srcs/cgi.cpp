#include "headers/cgi.hpp"

CGI::CGI() {
	_init = false;
}

void CGI::init(Server *server, Client *client, const char *path) {
	_server = server;
	_client = client;
	_path = strdup(path);
	_bodySize = 0;
	_init = true;
	setEnvironment();
	executeCGI();
}

CGI::~CGI() {}

bool CGI::isInit() {return _init;}

void CGI::setEnvironment() {
	std::map<std::string, std::string> env;

	queryStringParse(_client, _client->ReqGetPath());

	_clientHeader = _client->getBuff().substr(0, _client->getBuff().find(BODY_SEP));

	env["AUTH_TYPE"] = "basic";
	env["CONTENT_LENGTH"] = std::to_string(_client->ReqGetContentLength()); // nado proverit !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	env["CONTENT_TYPE"] = _client->ReqGetContentType();
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["PATH_INFO"] = _client->ReqGetPath();
	env["PATH_TRANSLATED"] = _client->ReqGetPath();
	env["QUERY_STRING"] = _client->ReqGetPath() + "?" + _queryString;
	env["REMOTE_ADDR"] = _server->getHost();
	env["REMOTE_IDENT"] = "basic";
	env["REMOTE_USER"] = "basic";
	env["REQUEST_METHOD"] = _client->ReqGetMethod();
	env["REQUEST_URI"] = _client->ReqGetPath();
	env["SCRIPT_NAME"] = _client->ReqGetPath();

    if (_server->getServerName().empty())
		env["SERVER_NAME"] = env["REMOTE_ADDR"];
	else
		env["SERVER_NAME"] = _server->getServerName();

	env["SERVER_PORT"] = _server->getPort();
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "KiRoTaMagic/6.9";
	env["HTTP_X_SECRET_HEADER_FOR_TEST"] = "1";

    // env.insert(_clientHeader.begin(), _clientHeader.end());
	// env.insert(_client->getBuff().substr(_client->getBuff().find(BODY_SEP)));

	setEnvToString(env);
	env.clear();
}

void CGI::queryStringParse(Client *client, std::string path)
{
	if (client->getBuff().find(CRLF) != std::string::npos)
	{
		size_t queryStringPos = 0;
		if ((queryStringPos = path.find('?')) != std::string::npos)
		{
			_queryString = std::string(path, queryStringPos + 1);
		}
	}
}

char **CGI::setEnvToString(std::map<std::string, std::string> env) {
	_environment = (char **)calloc(env.size() + 1, sizeof(char *));

	std::map<std::string, std::string>::iterator it;
	int i = 0;
	for (it = env.begin(); it != env.end(); it++, i++) {
		std::string str = it->first + "=" + it->second;
		_environment[i] = strdup(str.c_str());
	}
	return _environment;
}

void CGI::clean() {
	if (_init) {
		for (size_t i = 0; _environment[i]; i++) {
			if (_environment[i]) {
				free(_environment[i]);
				_environment[i] = nullptr;
			}
		}
		free(_environment);
		_environment = nullptr;
		free(_path);
		_path = nullptr;
	}
}

char **CGI::getEnvironment() const {return _environment;}

void	CGI::executeCGI() {

	int savedFd[2];
	FILE *file[2];
	int fd[2];
	pid_t pid;
	std::string newBody;

	if ((savedFd[IN] = dup(STDIN_FILENO)) == -1 || (savedFd[OUT] = dup(STDOUT_FILENO)) == -1)
		throw std::runtime_error(RED + std::string("Can't create file descriptor") + RESET);;
	if (!(file[IN] = tmpfile()) || !(file[OUT] = tmpfile()))
		throw std::runtime_error(RED + std::string("Can't create temporary file") + RESET);
	if ((fd[IN] = fileno(file[IN])) == -1 || (fd[OUT] = fileno(file[OUT])) == -1)
		throw std::runtime_error(RED + std::string("Can't create file descriptor") + RESET);

	write(fd[IN], _client->ReqGetContent().c_str(), _client->ReqGetContent().size());
	lseek(fd[IN], SEEK_SET, SEEK_SET);

	pid = fork();
	if (pid == -1) {
		_client->RespSetStatusCode("500");
		throw std::runtime_error(RED + std::string("Can't fork process") + RESET);
	}
	else if (pid == 0) {
		dup2(fd[IN], STDIN_FILENO);
		dup2(fd[OUT], STDOUT_FILENO);
		char * const * nlPointer = NULL;
		if (execve(_path, nlPointer, _environment) == -1)
			throw std::runtime_error(RED + std::string("Execve crashed!") + RESET);
		write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
	}
	else
	{
		char buffer[CGI_BUFSIZE];
		waitpid(-1, NULL, 0);
		lseek(fd[OUT], SEEK_SET, SEEK_SET);
		ssize_t bytes = 1;
		while (bytes > 0) {
			bzero(buffer, CGI_BUFSIZE);
			bytes = read(fd[OUT], buffer, CGI_BUFSIZE);
			_bodySize += bytes;
			newBody += buffer;
		}
	}
	dup2(savedFd[IN], STDIN_FILENO);
	dup2(savedFd[OUT], STDOUT_FILENO);
	fclose(file[IN]);
	fclose(file[OUT]);
	close(fd[IN]);
	close(fd[OUT]);
	close(savedFd[IN]);
	close(savedFd[OUT]);
	if (pid == 0)
		exit(0);
	size_t pos;
	std::string _clientHeader;
	if ((pos = newBody.find(BODY_SEP, 0)) != std::string::npos) {
		_clientHeader = std::string(newBody, 0, pos + 4);
		newBody = std::string(newBody, pos + 4);
		if (_clientHeader.find("Status: ", 0) != std::string::npos)
			_client->RespSetStatusCode(_clientHeader.substr(8, 3).c_str());
		if ((pos = _clientHeader.find("Content-Type: ", 0)) != std::string::npos)
			_client->ReqSetContentType(_clientHeader.substr(pos + 14, 24));
		// _client->setCgiHeader(_clientHeader); // ---------------------------------------------- ?????????????????????????????????????? cgi header
		_client->RespSetContentLength((size_t)_bodySize - _clientHeader.size());
	}
    // char *temp = (char *)malloc(sizeof(char) * newBody.size());
	char *temp = new char[newBody.size()];
    for (size_t i = 0; i < newBody.size(); ++i) {
        temp[i] = newBody[i];
    }
	_client->setBody(temp);
}
