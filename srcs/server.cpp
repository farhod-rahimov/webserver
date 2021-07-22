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

// LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION | LOCATION

Location::Location() {
    this->_autoindex = "false";
    this->_allowed_methods = "GET";
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
