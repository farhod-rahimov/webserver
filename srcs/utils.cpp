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