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