#include "./headers/Header.hpp"

void ft_get_method(std::string & _req_method, std::string & buf);
void ft_get_path(std::string & _req_path, std::string & _req_method, std::string & buf);
void ft_get_protocol(std::string & _req_protocol, std::string & _req_path, std::string & buf);
void ft_get_host(std::string & _req_host, std::string & buf);
void ft_get_connection(std::string & _req_connection, std::string & buf);

void ft_get_content_type(std::string & _req_content_type, std::string & buf);
void ft_get_content_length(Client & client, std::string & buf);
void ft_get_content(std::string & _req_content, std::string & buf);

void ft_parse_request(std::map<int, Client> & clients, int fd) {
    std::string buf = clients[fd].getBuff();

    std::cout << "\nCAME REQUEST\n";
    std::cout << "'" << buf << "'";
    ft_get_method(clients[fd].ReqGetMethod(), buf);
    ft_get_path(clients[fd].ReqGetPath(), clients[fd].ReqGetMethod(), buf);
    ft_get_protocol(clients[fd].ReqGetProtocol(), clients[fd].ReqGetPath(), buf);
    ft_get_host(clients[fd].ReqGetHost(), buf);
    ft_get_connection(clients[fd].ReqGetConnection(), buf);
    
    ft_get_content_type(clients[fd].ReqGetContentType(), buf); // needs to be tested
    ft_get_content_length(clients[fd], buf); // needs to be tested
    ft_get_content(clients[fd].ReqGetContent(), buf); // needs to be tested
};

void ft_get_method(std::string & _req_method, std::string & buf) {
    int i = buf.find("HTTP");
    size_t len;

    for (; i > 0 && buf[i - 1] != '\n'; i--) {
    }
    for (len = i; len < buf.size() && buf[len] != ' '; len++) {
    }
    _req_method = buf.substr(i, len - i);
    std::cout << "\nREQ_METHOD '" << _req_method << "'\n";
}

void ft_get_path(std::string & _req_path, std::string & _req_method, std::string & buf) {
    size_t i = buf.find(_req_method);
    size_t len;

    for (; i < buf.size() && buf[i] != ' '; i++) {
    }
    for (len = ++i; len < buf.size() && buf[len] != ' '; len++) {
    }
    _req_path = buf.substr(i, len - i);
    std::cout << "\nREQ_PATH '" << _req_path << "'\n";
}

void ft_get_protocol(std::string & _req_protocol, std::string & _req_path, std::string & buf) {
    size_t i = buf.find(_req_path);
    size_t len;

    for (; i < buf.size() && buf[i] != ' '; i++) {
    }
    for (len = ++i; len < buf.size() && buf[len] != '\r'; len++) {
    }
    _req_protocol = buf.substr(i, len - i);
    std::cout << "\nREQ_PROTOCOL '" << _req_protocol << "'\n";
}

void ft_get_host(std::string & _req_host, std::string & buf) {
    size_t i = buf.find("Host:");
    size_t len;

    for (; i < buf.size() && buf[i] != ' '; i++) {
    }
    for (len = ++i; len < buf.size() && buf[len] != '\r'; len++) {
    }
    _req_host = buf.substr(i, len - i);
    std::cout << "\nREQ_HOST '" << _req_host << "'\n";
}

void ft_get_connection(std::string & _req_connection, std::string & buf) {
    size_t i = buf.find("Connection:");
    size_t len;

    for (; i < buf.size() && buf[i] != ' '; i++) {
    }
    for (len = ++i; len < buf.size() && buf[len] != '\r'; len++) {
    }
    _req_connection = buf.substr(i, len - i);
    std::cout << "\nREQ_Connection '" << _req_connection << "'\n";
}

void ft_get_content_type(std::string & _req_content_type, std::string & buf) {
    size_t i = buf.find("Content-Type:");
    size_t len;

    if (i != buf.npos) {
        for (; i < buf.size() && buf[i] != ' '; i++) {
        }
        for (len = ++i; len < buf.size() && buf[len] != '\r'; len++) {
        }
        _req_content_type = buf.substr(i, len - i);
    }
    std::cout << "\nREQ_Content-Type '" << _req_content_type << "'\n";
};

void ft_get_content_length(Client & client, std::string & buf) {
    size_t i = buf.find("Content-Length:");
    size_t len;
    
    if (i != buf.npos) {
        for (; i < buf.size() && buf[i] != ' '; i++) {
        }
        for (len = ++i; len < buf.size() && buf[len] != '\r'; len++) {
        }
        client.ReqSetContentLength(std::atoi(buf.substr(i, len - i).c_str()));
    }
    std::cout << "\nREQ_Content-Length '" << client.ReqGetContentLength() << "'\n";
};

void ft_get_content(std::string & _req_content, std::string & buf) {
    size_t i = buf.find("Content:");
    size_t len;
    
    if (i != buf.npos) {
        for (; i < buf.size() && buf[i] != ' '; i++) {
        }
        for (len = ++i; len < buf.size() && buf[len] != '\r'; len++) {
        }
        _req_content = buf.substr(i, len - i);
    }
    std::cout << "\nREQ_Content '" << _req_content << "'\n";
};
