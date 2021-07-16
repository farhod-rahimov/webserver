#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"

Client::Client() {
    this->_request_num = 0;
    this->_response_num = 0;
};

Client::~Client() {};

Client::Client(const Client & src) {
    *this = src;
};
		
Client & Client::operator = (const Client & src) {
    if (this != &src) {
        this->_request = src._request;
        this->_request_num = src._request_num;
        this->_response = src._response;
        this->_response_num = src._response_num;
    }
    return (*this);
};

void Client::setRequest(Request req) {
    this->_request.insert(this->_request.end(), req);
    this->_request_num = this->_request.size();
};

void Client::setRequestNum(size_t num) {
    this->_request_num = num;
};

void Client::setResponse(Response resp) {
    this->_response.insert(this->_response.end(), resp);
    this->_response_num = this->_response.size();
};

void Client::setResponseNum(size_t num) {
    this->_response_num = num;
};
