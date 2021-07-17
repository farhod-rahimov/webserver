#include "Client.hpp"
#include <iostream>

Client::Client() {
    this->_req_content_length = 0;
    this->_req_num = 0;
    
    // this->_resp_content_length = 0;
    this->_resp_remained_to_sent = 0;
    this->_resp_num = 0;
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

void Client::ReqSetReqNum(size_t num) {
    this->_req_num = num;
};

size_t Client::ReqGetReqNum(void) {
    return (this->_req_num);
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

void Client::RespSetStatusTxt(std::string str) {
    this->_resp_status_txt.clear();
    this->_resp_status_txt = str;
};

std::string & Client::RespGetStatusTxt(void) {
    return (this->_resp_status_txt);
};

void Client::RespSetContentType(std::string str) {
    this->_resp_content_type.clear();
    this->_resp_content_type = "Content-Type: ";
    this->_resp_content_type.append(str);
};

std::string & Client::RespGetContentType(void) {
    return (this->_resp_content_type);
};

// void Client::RespSetContentLength(size_t len) {
//     this->_resp_content_length = len;
//     this->RespSetContentLengthTxt(len);
// }

// size_t Client::RespGetContentLength(void) {
//     return (this->_resp_content_length);
// };

void Client::RespSetContentLength(size_t len) {
    this->_resp_content_length.clear();
    this->_resp_content_length.append("Content-Length: ");
    this->_resp_content_length.append(std::to_string(len));
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

void Client::RespSetRespNum(size_t num) {
    this->_resp_num = num;
};

size_t Client::RespGetRespNum(void) {
    return (this->_resp_num);
};


std::string & Client::RespCreateFullRespTxt(void) {
    this->_resp_full_resp_txt.clear();
    this->_resp_full_resp_txt.append(this->_resp_protocol); this->_resp_full_resp_txt.append(" ");
    this->_resp_full_resp_txt.append(this->_resp_status_code); this->_resp_full_resp_txt.append(" ");
    this->_resp_full_resp_txt.append(this->_resp_status_txt); this->_resp_full_resp_txt.append("\n");
    this->_resp_full_resp_txt.append(this->_resp_content_length); this->_resp_full_resp_txt.append("\n");
    this->_resp_full_resp_txt.append(this->_resp_content_type); this->_resp_full_resp_txt.append("\n\n");
    this->_resp_full_resp_txt.append(this->_resp_content);
    
    this->_resp_remained_to_sent = this->_resp_full_resp_txt.length();
    // std::cout << "\n\nSENDING\n\n" << this->_resp_full_resp_txt << "\n";
    return (this->_resp_full_resp_txt);
};

std::string & Client::RespGetFullRespTxt(void) {
    return (this->_resp_full_resp_txt);
}

int Client::RespGetRemainedToSent(void) {
    return (this->_resp_remained_to_sent);
};

void Client::RespSetRemainedToSent(int len) {
    this->_resp_remained_to_sent = len;
};

std::string & Client::getBuff(void) {
    return (this->_buff);
}
