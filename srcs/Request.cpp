#include "Request.hpp"

Request::Request() {};

Request::Request(std::string method, std::string path, std::string proto_v) {
    this->_method = method;
    this->_path = path;
    this->_protocol_version = proto_v;
};

Request::~Request() {};

Request::Request(const Request & src) {
    *this = src;
};

Request & Request::operator = (const Request & src) {
    if (this != &src) {
        this->_method = src._method;
        this->_path = src._path;
        this->_protocol_version = src._protocol_version;
        this->_connection = src._connection;
        this->_content_type = src._content_type;
        this->_content_length = src._content_length;
        this->_content = src._content;
    }
    return (*this);
};

void Request::setMethod(std::string method) {
    this->_method = method;
};

void Request::setPath(std::string path) {
    this->_path = path;
};

void Request::setProtoVer(std::string version) {
    this->_protocol_version = version;
};

void Request::setConnection(std::string connection) {
    this->_connection = connection;
};

void Request::setContentType(std::string type) {
    this->_content_type = type;
};

void Request::setContentLength(std::string len) {
    this->_content_length = len;
};

void Request::setContent(std::string content) {
    this->_content = content;
};

std::string & Request::getMethod(void) {
    return (this->_method);
}

std::string & Request::getPath(void) {
    return (this->_path);
}

std::string & Request::getProtoVer(void) {
    return (this->_protocol_version);
}

std::string & Request::getConnection(void) {
    return (this->_connection);
};

std::string & Request::getContentType(void) {
    return (this->_content_type);
};

std::string & Request::getContentLength(void) {
    return (this->_content_length);
};

std::string & Request::getContent(void) {
    return (this->_content);
};
