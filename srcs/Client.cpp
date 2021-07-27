#include "./headers/Client.hpp"

Client::Client() {
    this->_req_content_length = 0;
    
    this->_resp_remained_to_sent = 0;
	this->_resp_protocol = "HTTP/1.1";
    this->_resp_connection = "Connection: keep-alive";
    this->_resp_status_code = "200";
    this->_resp_status_txt = "OK";
};

Client::~Client() {};

Client::Client(const Client & src) {
    *this = src;
};
		
Client & Client::operator = (const Client & src) {
    if (this != &src) {
            this->_buff = src._buff;

            this->_req_method = src._req_method;
            this->_req_path = src._req_path;
            this->_req_protocol = src._req_protocol;
            this->_req_host = src._req_host;
            this->_req_connection = src._req_connection;
            
            this->_req_content_file_name = src._req_content_file_name;
            this->_req_content_type = src._req_content_type;
            this->_req_content_length = src._req_content_length;
            this->_req_content = src._req_content;
			
            this->_resp_protocol = src._resp_protocol;
            this->_resp_status_code = src._resp_status_code;
            this->_resp_status_txt = src._resp_status_txt;

            this->_resp_connection = src._resp_connection;
            this->_resp_location = src._resp_location;
            
            this->_resp_content_type = src._resp_content_type;
            this->_resp_content_length = src._resp_content_length;
            this->_resp_content = src._resp_content;

            this->_resp_full_resp_txt = src._resp_full_resp_txt;
            this->_resp_remained_to_sent = src._resp_remained_to_sent;
    }
    return (*this);
};

        // COMMON	|	COMMON	|	COMMON	|	COMMON	|	COMMON	|	COMMON

std::string & Client::getBuff(void) {
    return (this->_buff);
};

        // REQUEST | REQUEST | REQUEST | REQUEST | REQUEST | REQUEST | REQUEST

void Client::ReqSetMethod(std::string str) {
    this->_req_method = str;
};

std::string & Client::ReqGetMethod(void) {
    return (this->_req_method);
};

void Client::ReqSetPath(std::string str) {
    this->_req_path = str;
};

std::string & Client::ReqGetPath(void) {
    return (this->_req_path);
};

void Client::ReqSetProtocol(std::string str) {
    this->_req_protocol = str;
};

std::string & Client::ReqGetProtocol(void) {
    return (this->_req_protocol);
};

void Client::ReqSetHost(std::string str) {
    this->_req_host = str;
};

std::string & Client::ReqGetHost(void) {
    return (this->_req_host);
};

void Client::ReqSetConnection(std::string str) {
    this->_req_connection = str;
};

std::string & Client::ReqGetConnection(void) {
    return (this->_req_connection);
};

void Client::ReqSetContentFileName(std::string str) {
    this->_req_content_file_name = str;
};

std::string & Client::ReqGetContentFileName(void) {
    return (this->_req_content_file_name);
};

void Client::ReqSetContentType(std::string str) {
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
    this->_req_content = str;
};

std::string & Client::ReqGetContent(void) {
    return (this->_req_content);
};

        // RESPONSE | RESPONSE | RESPONSE | RESPONSE | RESPONSE | RESPONSE | RESPONSE

void Client::RespSetProtocol(std::string str) {
    this->_resp_protocol = str;
};

std::string & Client::RespGetProtocol(void) {
    return (this->_resp_protocol);
};

void Client::RespSetStatusCode(std::string str) {
    this->_resp_status_code = str;
};

std::string & Client::RespGetStatusCode(void) {
    return (this->_resp_status_code);
};

void Client::RespSetStatusTxt(std::string str) {
    this->_resp_status_txt = str;
};

std::string & Client::RespGetStatusTxt(void) {
    return (this->_resp_status_txt);
};

void Client::RespSetConnection(std::string str) {
    this->_resp_connection = str;
};

std::string & Client::RespGetConnection(void) {
    return (this->_resp_connection);
};

void Client::RespSetLocation(std::string str) {
    this->_resp_location = "Location: ";
    this->_resp_location.append(str);
};

std::string & Client::RespGetLocation(void) {
    return (this->_resp_location);
};

void Client::RespSetContentType(std::string str) {
    this->_resp_content_type = "Content-Type: ";
    this->_resp_content_type.append(str);
};

std::string & Client::RespGetContentType(void) {
    return (this->_resp_content_type);
};

void Client::RespSetContentLength(size_t len) {
    this->_resp_content_length = "Content-Length: ";;
    this->_resp_content_length.append(std::to_string(len));
};

std::string & Client::RespGetContentLength(void) {
    return (this->_resp_content_length);
};

void Client::RespSetContent(std::string str) {
    this->_resp_content = str;
};

std::string & Client::RespGetContent(void) {
    return (this->_resp_content);
};

std::string & Client::RespCreateFullRespTxt(void) {
    this->_resp_full_resp_txt.clear();
    this->_resp_full_resp_txt.append(this->_resp_protocol); this->_resp_full_resp_txt.append(" ");
    this->_resp_full_resp_txt.append(this->_resp_status_code); this->_resp_full_resp_txt.append(" ");
    this->_resp_full_resp_txt.append(this->_resp_status_txt); this->_resp_full_resp_txt.append("\n");
    if (this->_resp_location.length()) {
        this->_resp_full_resp_txt.append(this->_resp_location); this->_resp_full_resp_txt.append("\n");
    }
    if (this->_resp_content_length.length()) {
        this->_resp_full_resp_txt.append(this->_resp_content_length); this->_resp_full_resp_txt.append("\n");
        this->_resp_full_resp_txt.append(this->_resp_content_type); this->_resp_full_resp_txt.append("\n");
    }
    this->_resp_full_resp_txt.append("\n");
    if (this->_resp_content_length.length()) {
        this->_resp_full_resp_txt.append(this->_resp_content);
    }
    this->_resp_remained_to_sent = this->_resp_full_resp_txt.length();
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
