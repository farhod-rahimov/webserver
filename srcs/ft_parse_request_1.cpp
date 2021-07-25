#include "./headers/Header.hpp"

static void ft_get_method(std::string & _req_method, std::string & buf);
static void ft_get_path(std::string & _req_path, std::string & _req_method, std::string & buf);
static void ft_get_protocol(std::string & _req_protocol, std::string & _req_path, std::string & buf);
static void ft_get_host(std::string & _req_host, std::string & buf);
static void ft_get_connection(std::string & _req_connection, std::string & buf);

static void ft_get_content_type(std::string & _req_content_type, std::string & buf);
static void ft_get_content_length(Client & client, std::string & buf);
static void ft_get_content(std::string & _req_content, std::string & buf);

void ft_get_content_and_file_name(Client & client);
std::string ft_get_file_name(std::string & str, size_t pos);

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
    ft_get_content_and_file_name(clients[fd]);
};

void ft_get_method(std::string & _req_method, std::string & buf) {
    size_t i = buf.find("HTTP");
    size_t len;

    if (i != buf.npos) {
        for (; i > 0 && buf[i - 1] != '\n'; i--) {
        }
        for (len = i; len < buf.size() && buf[len] != ' '; len++) {
        }
        _req_method = buf.substr(i, len - i);
    }
    std::cout << "\nREQ_METHOD '" << _req_method << "'\n";
}

void ft_get_path(std::string & _req_path, std::string & _req_method, std::string & buf) {
    size_t i = buf.find(_req_method);
    size_t len;

    if (i != buf.npos) {
        for (; i < buf.size() && buf[i] != ' '; i++) {
        }
        for (len = ++i; len < buf.size() && buf[len] != ' '; len++) {
        }
        _req_path = buf.substr(i, len - i);
        for (size_t pos = _req_path.find("%20"); pos != _req_path.npos; pos = _req_path.find("%20")) {
            _req_path.replace(pos, 3, " ");
        }
    }
    std::cout << "\nREQ_PATH '" << _req_path << "'\n";
}

void ft_get_protocol(std::string & _req_protocol, std::string & _req_path, std::string & buf) {
    size_t i = buf.find(_req_path);
    size_t len;

    if (i != buf.npos) {
        for (; i < buf.size() && buf[i] != ' '; i++) {
        }
        for (len = ++i; len < buf.size() && buf[len] != '\r'; len++) {
        }
        _req_protocol = buf.substr(i, len - i);
    }
    std::cout << "\nREQ_PROTOCOL '" << _req_protocol << "'\n";
}

void ft_get_host(std::string & _req_host, std::string & buf) {
    size_t i = buf.find("Host:");
    size_t len;

    if (i != buf.npos) {
        for (; i < buf.size() && buf[i] != ' '; i++) {
        }
        for (len = ++i; len < buf.size() && buf[len] != '\r'; len++) {
        }
        _req_host = buf.substr(i, len - i);
    }
    std::cout << "\nREQ_HOST '" << _req_host << "'\n";
}

void ft_get_connection(std::string & _req_connection, std::string & buf) {
    size_t i = buf.find("Connection:");
    size_t len;

    if (i != buf.npos) {
        for (; i < buf.size() && buf[i] != ' '; i++) {
        }
        for (len = ++i; len < buf.size() && buf[len] != '\r'; len++) {
        }
        _req_connection = buf.substr(i, len - i);
    }
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
    size_t c_len = buf.find("Content-Length:");
    
    // size_t i = buf.find("Content: ") + 4;
    size_t i = buf.find("\r\n\r\n") + 4;

    if (c_len != buf.npos && i != buf.npos && i < buf.size()) {
        _req_content = buf.substr(i);
    }
    std::cout << "\nREQ_Content '" << _req_content << "'\n";
};

void ft_get_new_content(std::string & _req_content, std::string & buf);

void ft_get_content_and_file_name(Client & client) { // если это не отправка файла, но return
    size_t file_name_pos = file_name_pos = client.ReqGetContent().find("filename=\"");
    
    if (client.ReqGetContent().empty() || (file_name_pos == client.ReqGetContent().npos))
        return ;

    client.ReqSetContentFileName(ft_get_file_name(client.ReqGetContent(), file_name_pos));
    ft_get_content_type(client.ReqGetContentType(), client.ReqGetContent());

    std::string tmp = client.ReqGetContent().substr(client.ReqGetContent().find("\r\n\r\n") + 4);
    ft_get_new_content(client.ReqGetContent(), tmp);
}

std::string ft_get_file_name(std::string & str, size_t pos) {
    std::string ret;
    size_t start_pos;
    
    pos += strlen("filename\"") + 1;
    start_pos = pos;

    for (; str[pos] != '"'; pos++) {}

    ret = str.substr(start_pos, pos - start_pos);
    std::cout << "\nREQ_FILE_NAME '" << ret << "'\n";
    return (ret);
}

void ft_get_new_content(std::string & _req_content, std::string & buf) {
    size_t end = buf.find("\r\n");

    if (end != buf.npos) {
        _req_content = buf.substr(0, end);
    }
    std::cout << "\nREQ_NEW_Content '" << _req_content << "'\n";
}