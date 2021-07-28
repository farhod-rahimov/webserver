#include "./headers/Header.hpp"

void ft_create_new_write_event(int kq, int fd) {
    struct kevent tmp;
    EV_SET(&tmp, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    kevent(kq, &tmp, 1, NULL, 0, NULL);
}

void ft_send_ok(Client & client) {
	client.RespSetStatusCode("200");
	client.RespSetStatusTxt("OK");
	client.RespCreateFullRespTxt();	
}

void ft_send_204_not_content(Client & client) {
	client.RespSetStatusCode("204");
	client.RespSetStatusTxt("NO CONTENT");
	client.RespCreateFullRespTxt();	
}

void ft_send_forbidden(Client & client) {
	const char * content = "<html>\nError 403 Forbidden.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetStatusCode("403");
	client.RespSetStatusTxt("FORBIDDEN");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

void ft_send_internal_error(Client & client) {
	const char * content = "<html>\nError 500 Internal Server Error.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetStatusCode("500");
	client.RespSetStatusTxt("INTERNAL SERVER ERROR");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

void ft_send_not_found(Client & client) {
	const char * content = "<html>\nError 404 Not Found.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetStatusCode("404");
	client.RespSetStatusTxt("NOT FOUND");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

void ft_send_too_long_body(Client & client, int fd, int kq) {
	const char * content = "<html>\nError 413 Request Entity Too Large.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetStatusCode("413");
	client.RespSetStatusTxt("Request Entity Too Large");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();

    send(fd, client.RespGetFullRespTxt().c_str(), client.RespGetRemainedToSent(), 0);

    struct kevent tmp;
    EV_SET(&tmp, fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
    kevent(kq, &tmp, 1, NULL, 0, NULL);
}

void ft_create_header(const char * content_type, Client & client, std::string & content) {

    client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("200");
	client.RespSetStatusTxt("OK");
	client.RespSetContentType(content_type);
	client.RespSetContentLength(content.length());
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();
}

void ft_send_file_was_created(Client & client) {
	const char * content = "<html>\nYour file was created. It is here <p><a href=\"/uploaded_files\">Your file</a></p>\n</html>";
	
	client.RespSetStatusCode("201");
	client.RespSetStatusTxt("CREATED");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

void ft_send_not_implemented(Client & client) {
	const char * content = "<html>\nError 501 Not Implemented.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("501");
	client.RespSetStatusTxt("NOT IMPLEMENTED");
	client.RespSetConnection("Connection: keep-alive");
	client.RespSetContentType("text/html; charset=utf-8");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

void ft_send_protocol_not_supported(Client & client) {
	const char * content = "<html>\nError 505 HTTP Version Not Supported.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("505");
	client.RespSetStatusTxt("HTTP VERSION NOT SUPPORTED");
	client.RespSetConnection("Connection: keep-alive");
	client.RespSetContentType("text/html; charset=utf-8");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

void ft_send_method_not_allowed(Client & client) {
	const char * content = "<html>\nError 405 Method Not Allowed.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("405");
	client.RespSetStatusTxt("METHOD NOT ALLOWED");
	client.RespSetConnection("Connection: keep-alive");
	client.RespSetContentType("text/html; charset=utf-8");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

// ------------------------------------------- //

std::string ft_get_req_path_extension(Client & client) {
    std::string ext;
    size_t i = 0, pos = client.ReqGetPath().rfind(".");

    if (pos != std::string::npos) {
        ext = client.ReqGetPath().substr(pos);
        if (ext.back() == '/')
            ext.pop_back();
    }
    if (ext[i] == '.')
        i++;
    for (; ext[i] >= 'a' && ext[i] <= 'z'; i++) {}
    ext = ext.substr(0, i);
    return (ext);
}

int ft_work_with_cgi(Client & client, Server & server, Location & location, int fd) {
    Cgi cgi(client, server, location, fd);
    
    return (cgi.cgiInit());
}
