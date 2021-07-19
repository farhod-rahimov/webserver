#include "Header.hpp"

void ft_create_my_def_response(Client & client) {
	client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("200");
	client.RespSetStatusTxt("OK");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(static_cast<const char *>("<html>\nPOKA\n</html>")));
	client.RespSetContent("<html>\nPOKA\n</html>");
	
	client.RespCreateFullRespTxt();
}

void ft_send_not_implemented(Client & client) {
	const char * content = "<html>\nError 501 Not Implemented\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("501");
	client.RespSetStatusTxt("NOT IMPLEMENTED");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

void ft_send_response(std::map<int, Client> & clients, int fd) {
	
	ft_create_response(clients[fd]);
    int ret = send(fd, clients[fd].RespGetFullRespTxt().c_str(), clients[fd].RespGetRemainedToSent(), 0);
		if (ret < 0) std::cout << "SEND ERROR\n"; else std::cout << "SEND OK\n";
	std::cout << "\nSENT\n'" << clients[fd].RespGetFullRespTxt() << "'\n";
}

int ft_check_method(std::string & _method);

void ft_create_response(Client & client) {
	if (ft_check_method(client.ReqGetMethod()) == -1)
		return (ft_send_not_implemented(client));
	else
		ft_create_my_def_response(client);
}

int ft_check_method(std::string & _method) {
	if (!_method.compare("GET"))
		return (1);
	else if (!_method.compare("POST"))
		return (2);
	else if (!_method.compare("DELETE"))
		return (3);
	return (-1);
}