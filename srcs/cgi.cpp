#include "./headers/Cgi.hpp"

#include <string.h>

Cgi::Cgi(Client & client, Server & server, Location & location, int fd) {
    this->_resp_client = client;
    this->_resp_server = server;
    this->_resp_location = location;
    this->_fd = fd;
};

Cgi::~Cgi() {
    this->cgiClearEnv();
};

Cgi::Cgi(const Cgi & src) {
    *this = src;
};

Cgi & Cgi::operator = (const Cgi & src) {
    if (this != &src) {
        this->_resp_client = src._resp_client;
        this->_resp_server = src._resp_server;
        this->_resp_location = src._resp_location;
        this->_env = src._env;
        this->_fd = src._fd;
        this->_tmp_env = src._tmp_env;
    }
    return (*this);
};

void Cgi::cgiInit(void) {
    this->cgiCreateEnv();
    this->cgiConvertEnv();
    this->cgiExecute();
};

void Cgi::cgiCreateEnv(void) {
    std::map<std::string, std::string> & tmp_env = this->_tmp_env;


    tmp_env["SERVER_SOFTWARE="] = "beast_server 1.0";
    tmp_env["SERVER_NAME="] = this->_resp_server.getHost();
    tmp_env["GATEWAY_INTERFACE="] = "beast_cgi 1.0";
    tmp_env["SERVER_PROTOCOL="] = this->_resp_client.ReqGetProtocol();
    tmp_env["SERVER_PORT="] = this->_resp_server.getPort();
    tmp_env["REQUEST_METHOD="] = this->_resp_client.ReqGetMethod();
    
    std::string path_info;
    std::string path = _resp_client.ReqGetPath();
    size_t pos = path.find(this->_resp_location.getCgiExtension());
    
    if (path.back() != '/') {path.append("/");}
    if (pos != std::string::npos) {pos += this->_resp_location.getCgiExtension().length();}
    if (path.length() > pos + 1) {path_info = path.substr(pos + 1);}
    
    tmp_env["PATH_INFO="] = path_info;
    
    if (!path_info.empty())
    tmp_env["PATH_TRANSLATED="] = path;
    
    tmp_env["SCRIPT_NAME="] = this->_resp_location.getCgiPath();
    
    pos = this->_resp_client.ReqGetPath().find("?");
    if (pos != std::string::npos)
        tmp_env["QUERY_STRING="] = this->_resp_client.ReqGetPath().substr(pos + 1);
    else {
        tmp_env["QUERY_STRING="] = this->_resp_client.ReqGetContent();
        tmp_env["CONTENT_TYPE="] = this->_resp_client.ReqGetContentType();
        tmp_env["CONTENT_LENGTH="] = this->_resp_client.ReqGetContentLength();
    }
};

void Cgi::cgiConvertEnv(void) {
    std::string tmp;
    this->_env = new char * [this->_tmp_env.size() + 1];

    int i = 0;
    for (std::map<std::string, std::string>::iterator it = this->_tmp_env.begin(); \
            it != this->_tmp_env.end(); it++) {
        tmp = it->first + it->second;
        this->_env[i++] = strdup(tmp.c_str());
    }
    this->_env[i] = NULL;
};

void Cgi::cgiExecute(void) {
    pid_t   child;
    int     status;
    int     initial_1;

    if ((initial_1 = dup(1)) == -1) {
        std::cerr << "Dup error\n" ; return ;
    }
    if (dup2(this->_fd, 1) == -1) {
        std::cerr << "Dup2 error\n" ; return ;
    }

    // ------------------------------------------------------------------------ //

    if ((child = fork()) == 0) {
        if ((execve(this->_resp_location.getCgiPath().c_str(), NULL, this->_env)) == -1) {
            std::cerr << "Execve error\n"; exit(1);
        }
    }
    else if (child == -1) {
        std::cerr << "Fork error\n" ; return ;
    }
    else {
        waitpid(child, &status, 0);
    }
    if (dup2(initial_1, 1) == -1) {
        std::cerr << "Dup2 error\n" ; return ;
    }

    // ------------------------------------------------------------------------ //

};

void Cgi::cgiClearEnv(void) {
    for (size_t i = 0; this->_env[i] != NULL; i++) {
        delete this->_env[i];
    }
    delete [] this->_env;
};
