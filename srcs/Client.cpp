#include "Client.hpp"

Client::Client() {
};

Client::~Client() {};

Client::Client(const Client & src) {
    *this = src;
};
		
Client & Client::operator = (const Client & src) {
    if (this != &src) {
    }
    return (*this);
};

        // REQUEST | REQUEST | REQUEST | REQUEST | REQUEST | REQUEST | REQUEST

void Client::ReqSetMethod(std::string str) {
    this->_req_method.clear();
    this->_req_method = str;
};

std::string & Client::ReqGetMethod(void) {
    return (this->_req_method);
};

void Client::ReqSetPath(std::string str) {
    this->_req_path.clear();
    this->_req_path = str;
};

std::string & Client::ReqGetPath(void) {
    return (this->_req_path);
};

void Client::ReqSetProtocol(std::string str) {
    this->_req_protocol.clear();
    this->_req_protocol = str;
};

std::string & Client::ReqGetProtocol(void) {
    return (this->_req_protocol);
};

void Client::ReqSetConnection(std::string str) {
    this->_req_connection.clear();
    this->_req_connection = str;
};

std::string & Client::ReqGetConnection(void) {
    return (this->_req_connection);
};

void Client::ReqSetContentType(std::string str) {
    this->_req_content_type.clear();
    this->_req_content_type = str;
};

std::string & Client::ReqGetContentType(void) {
    return (this->_req_content_type);
};

void Client::ReqSetContentLength(size_t n) {
    this->_req_content_length = n;
};

size_t Client::ReqGetContentLength(void) {
    return (this->_req_content_length);
};


void Client::ReqSetContent(std::string str) {
    this->_req_content.clear();
    this->_req_content = str;
};

std::string & Client::ReqGetContent(void) {
    return (this->_req_content);
};

        // RESPONSE | RESPONSE | RESPONSE | RESPONSE | RESPONSE | RESPONSE | RESPONSE

void Client::RespSetProtocol(std::string str) {
    this->_resp_protocol.clear();
    this->_resp_protocol = str;
};

std::string & Client::RespGetProtocol(void) {
    return (this->_resp_protocol);
};

void Client::RespSetStatusCode(std::string str) {
    this->_resp_status_code.clear();
    this->_resp_status_code = str;
};

std::string & Client::RespGetStatusCode(void) {
    return (this->_resp_status_code);
};

void Client::RespSetStatusText(std::string str) {
    this->_resp_status_text.clear();
    this->_resp_status_text = str;
};

std::string & Client::RespGetStatusText(void) {
    return (this->_resp_status_text);
};

void Client::RespSetContentType(std::string str) {
    this->_req_content_type.clear();
    this->_req_content_type = "Content-Type: ";
    this->_req_content_type.append(str);
};

std::string & Client::RespGetContentType(void) {
    return (this->_req_content_type);
};

void Client::RespSetContentLength(std::string str) {
    this->_resp_content_length.clear();
    this->_resp_content_length = "Content-Length: ";
    this->_resp_content_length.append(str);
};

std::string & Client::RespGetContentLength(void) {
    return (this->_resp_content_length);
};

void Client::RespSetContent(std::string str) {
    this->_resp_content.clear();
    this->_resp_content = str;
};

std::string & Client::RespGetContent(void) {
    return (this->_resp_content);
};
