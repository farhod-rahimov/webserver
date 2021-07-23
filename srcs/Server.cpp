#include "./headers/Server.hpp"

// SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER

Server::Server() {
    this->_check_flag = true;
};

Server::~Server() {};

Server::Server(const Server & src) {
    this->_check_flag = true;
    *this = src;
};

Server & Server::operator = (const Server & src) {          // complete it
    if (this != &src) {
        this->_check_flag = src._check_flag;
        this->_host = src._host;
        this->_port = src._port;
        this->_server_name = src._server_name;
        this->_server_root = src._server_root;
        this->_default_error_status_code = src._default_error_status_code;
        this->_default_error_page_path = src._default_error_page_path;
        this->_limit_body_size = src._limit_body_size;
       
        this->_locations = src._locations;
        this->_clients = src._clients;
    }
    return (*this);
};

std::string & Server::getHost(void) {
    return (this->_host);
};

std::string & Server::getPort(void) {
    return (this->_port);
};

std::string & Server::getServerName(void) {
    return (this->_server_name);
};

std::string & Server::getServerRoot(void) {
    return (this->_server_root);
};

std::string & Server::getDefaultErrorStatusCode(void) {
    return (this->_default_error_status_code);
};

std::string & Server::getDefaultErrorPagePath(void) {
    return (this->_default_error_page_path);
};

std::string & Server::getLimitBodySize(void) {
    return (this->_limit_body_size);
};

std::vector<location> & Server::getLocations(void) {
    return (this->_locations);
};

std::map<int, Client> & Server::getClients(void) {
    return (this->_clients);
};

bool & Server::getCheckFlag(void) {
    return (this->_check_flag);
};

// LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION

Location::Location() {
    this->_autoindex = "off";
    this->_allowed_methods = "GET";
};

Location::Location(const Location * src) {
    this->_autoindex = "off";
    this->_allowed_methods = "GET";
    *this = src;
};

Location & Location::operator = (const Location & src) {
    if (this != &src) {
        this->_allowed_methods = src._allowed_methods;
        this->_redirection = src._redirection;
        this->_redirection_status_code = src._redirection_status_code;
        this->_path = src._path;
        this->_autoindex = src._autoindex;
        this->_default_file = src._default_file;
        this->_cgi_path = src._cgi_path;
        this->_cgi_extension = src._cgi_extension;
        this->_location_root = src._location_root;
        this->_upload_directory = src._upload_directory;
    }
    return(*this);
};

std::string & Location::getAllowedMethods(void) {
    return (this->_allowed_methods);
};

std::string & Location::getRedirection(void) {
    return (this->_redirection);
};

std::string & Location::getRedirectionStatusCode(void) {
    return (this->_redirection_status_code);
};

std::string & Location::getPath(void) {
    return (this->_path);
};

std::string & Location::getAutoindex(void) {
    return (this->_autoindex);
};

std::string & Location::getDefaultFile(void) {
    return (this->_default_file);
};

std::string & Location::getCgiPath(void) {
    return (this->_cgi_path);
};

std::string & Location::getCgiExtension(void) {
    return (this->_cgi_extension);
};

std::string & Location::getLocationRoot(void) {
    return (this->_location_root);
};

std::string & Location::getUploadDirectory(void) {
    return (this->_upload_directory);
};
