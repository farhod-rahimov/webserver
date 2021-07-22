#include "./headers/Server.hpp"

// SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER | SERVER

Server::Server() {};

Server::~Server() {};

Server::Server(const Server & src) {
    *this = src;
};

Server & Server::operator = (const Server & src) {          // complete it
    if (this != &src) {
    }
    return (*this);
};

std::string & Server::GetHost(void) {
    return (this->_host);
};

std::string & Server::GetPort(void) {
    return (this->_port);
};

std::string & Server::GetServerName(void) {
    return (this->_server_name);
};

std::string & Server::GetServerRoot(void) {
    return (this->_server_root);
};

std::string & Server::GetDefaultErrorPage(void) {
    return (this->_default_error_page);
};

std::string & Server::GetLimitBodySize(void) {
    return (this->_limit_body_size);
};

std::vector<location> & Server::GetLocations(void) {
    return (this->_locations);
};

std::map<int, Client> & Server::GetClients(void) {
    return (this->_clients);
};

// LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION

Location::Location() {
    this->_autoindex = false;
    this->_upload_accept = false;
};

std::string & Location::GetAllowMethods(void) {
    return (this->_allow_methods);
};

std::string & Location::GetRedirection(void) {
    return (this->_redirection);
};

std::string & Location::GetPath(void) {
    return (this->_path);
};

bool        & Location::GetAutoindex(void) {
    return (this->_autoindex);
};

std::string & Location::GetDefaultFile(void) {
    return (this->_default_file);
};

std::string & Location::GetCgiPath(void) {
    return (this->_cgi_path);
};

std::string & Location::GetLocationRoot(void) {
    return (this->_location_root);
};

bool        & Location::GetUploadAccept(void) {
    return (this->_upload_accept);
};

std::string & Location::GetUploadDirectory(void) {
    return (this->_upload_directory);
};
