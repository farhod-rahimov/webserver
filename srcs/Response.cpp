#include "Response.hpp"

Response::Response() {
    this->_protocol_version = "HTTP/1.1";
    
    this->_status_code = "200";
    this->_status_text = "OK";

    this->_connection = "Connection: keep-alive";
    this->_content_type = "Content-type: text/html";
    this->_content_length = "Content-length: 0";            // last header, after him goest _content
};

Response::~Response() {};

Response::Response(const Response & src) {
    *this = src;
};

Response & Response::operator = (const Response & src) {
    if (this != &src) {
        this->_protocol_version = src._protocol_version;
        
        this->_status_code = src._status_code;
        this->_status_text = src._status_text;
        
        this->_connection = src._connection;
        
        this->_content_type = src._content_type;
        this->_content_length = src._content_length;
        this->_content = src._content;
    }
    return (*this);
};

void Response::setProtoVer(std::string version) {
    this->_protocol_version = version;
};

void Response::setStatusCode(std::string status_code) {
    this->_status_code =status_code;
};

void Response::setStatusText(std::string status_text) {
    this->_status_text = status_text;
};

void Response::setConnection(std::string connection) {
    this->_connection = connection;
};

void Response::setContentType(std::string type) {
    this->_content_type = type;
};

void Response::setContentLength(std::string len) {
    this->_content_length = len;
};

void Response::setContent(std::string content) {
    this->_content = content;
};
