#include "Header.hpp"

void ft_create_response(std::map<int, Client> & clients, int fd) {
	clients[fd].RespSetProtocol("HTTP/1.1");
	clients[fd].RespSetStatusCode("200");
	clients[fd].RespSetStatusTxt("OK");
	clients[fd].RespSetContentType("text/html");
	clients[fd].RespSetContentLength(strlen(static_cast<const char *>("<html>\nPOKA\n</html>")));
	clients[fd].RespSetContent("<html>\nPOKA\n</html>");
	
	clients[fd].RespCreateFullRespTxt();
}