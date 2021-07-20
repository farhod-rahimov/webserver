#include "./headers/Header.hpp"

void ft_create_my_def_response(Client & client) {
	client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("200");
	client.RespSetStatusTxt("OK");
	client.RespSetConnection("Connection: keep-alive");
	client.RespSetContentType("text/html; charset=utf-8");
	client.RespSetContentLength(strlen(static_cast<const char *>("<html>\nPOKA\n</html>")));
	client.RespSetContent("<html>\nPOKA\n</html>");
	
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

void ft_send_not_found(Client & client) {
	const char * content = "<html>\nError 404 Not Found.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("404");
	client.RespSetStatusTxt("NOT FOUND");
	client.RespSetConnection("Connection: keep-alive");
	client.RespSetContentType("text/html; charset=utf-8");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

void ft_send_response(std::map<int, Client> & clients, size_t fd, std::vector<struct kevent> & chlist) {
	int ret;
	(void)ret;
	ft_create_response(clients[fd]);
    
	ret = send(fd, clients[fd].RespGetFullRespTxt().c_str(), clients[fd].RespGetRemainedToSent(), 0);
	// if (ret < 0) std::cout << "SEND ERROR\n"; else std::cout << "SEND OK\n";
	// std::cout << "\nSENT\n'" << clients[fd].RespGetFullRespTxt() << "'\n";
	
	if (clients[fd].RespGetConnection().find("close") != clients[fd].RespGetConnection().npos) {
		size_t i = 0;
		for (; i < chlist.size() && chlist[i].ident != fd; i++) {}
		close(fd);
		chlist.erase(chlist.begin() + i);
	}
}

int ft_check_method(std::string & _method);
int ft_check_protocol(std::string & _protocol);
void ft_set_connection(Client & client);

void ft_create_response(Client & client) {
	int method;

	if ((method = ft_check_method(client.ReqGetMethod())) == -1 || ft_check_protocol(client.ReqGetProtocol()) == -1)
		return (ft_send_not_implemented(client));
	ft_set_connection(client);
	if (method == 1)
		ft_response_to_get(client);
	// else if (method == 2)
	// 	ft_response_to_post();
	// else
	// 	ft_response_to_delete();
	
	// ft_create_my_def_response(client);
}

int ft_check_method(std::string & _method) {
	if (_method.find("GET") != _method.npos)
		return (1);
	else if (_method.find("POST") != _method.npos)
		return (2);
	else if (_method.find("DELETE") != _method.npos)
		return (3);
	return (-1);
}

int ft_check_protocol(std::string & _protocol) {
	size_t pos = _protocol.find("HTTP/1.1");
	
	if (pos == _protocol.npos)
		return (-1);
	return (1);
}

void ft_set_connection(Client & client) {
	size_t pos = client.ReqGetConnection().find("close");

	if (pos != client.ReqGetConnection().npos)
		client.RespSetConnection("Connection: close");
	else
		client.RespSetConnection("Connection: keep-alive");
}