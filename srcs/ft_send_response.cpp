#include "./headers/Header.hpp"

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

void ft_send_response(Server & server, size_t fd, std::vector<struct kevent> & chlist, std::vector<Server> & servers) {
	int ret;
	(void)ret;
	std::map<int, Client> & clients = server.getClients();
	ft_create_response(clients[fd], servers, server);
    
	ret = send(fd, clients[fd].RespGetFullRespTxt().c_str(), clients[fd].RespGetRemainedToSent(), 0);
	// if (ret < 0) std::cout << "SEND ERROR\n"; else std::cout << "SEND OK\n";
	std::cout << "\nSENT\n'" << clients[fd].RespGetFullRespTxt() << "'\n";
	
	if (clients[fd].RespGetConnection().find("close") != clients[fd].RespGetConnection().npos) {
		size_t i = 0;
		for (; i < chlist.size() && chlist[i].ident != fd; i++) {}
		close(fd);
		chlist.erase(chlist.begin() + i);
	}
}
void ft_get_responding_server(std::vector<Server> & servers, Client & client, Server & responding_server);
void ft_get_responding_location(std::vector<Location> & locations, Location & responding_location, std::string & req_path);
size_t ft_get_max_match(std::string & s1, std::string & s2);
void ft_replace_req_path(std::string & req_path, std::string & location_path, std::string & location_root);
void ft_get_location_of_redirected_src(Location & responding_location, Client & client);

int ft_get_method(std::string req_method, std::string supported_methods);
int ft_check_protocol(std::string req_protocol);
void ft_set_connection_header(Client & client);

void ft_create_response(Client & client, std::vector<Server> & servers, Server responding_server) {
	int method;
	Location responding_location;
	
	ft_get_responding_server(servers, client, responding_server);
	ft_get_responding_location(responding_server.getLocations(), responding_location, client.ReqGetPath());
	if (responding_location.getRedirection().empty())
		ft_replace_req_path(client.ReqGetPath(), responding_location.getPath(), responding_location.getLocationRoot());
	else
		ft_get_location_of_redirected_src(responding_location, client);
	ft_set_connection_header(client);

	if (((method = ft_get_method(client.ReqGetMethod(), responding_location.getAllowedMethods())) < 0) || ft_check_protocol(client.ReqGetProtocol()) < 0)
		return (ft_send_not_implemented(client));
	
	if (method == 1) {ft_response_to_get(client, responding_server, responding_location);}
	// else if (method == 2) {ft_response_to_post();}
	// else if (method == 3) {ft_response_to_delete();}

	// exit(1);
}

void ft_get_responding_server(std::vector<Server> & servers, Client & client, Server & responding_server) {
	
	std::string req_server_name;

	if (client.ReqGetHost().find(":") != client.ReqGetHost().npos)
		req_server_name = client.ReqGetHost().substr(0, client.ReqGetHost().find(":"));
	else
		req_server_name = client.ReqGetHost();
	// std::cout << "req_host " << req_server_name << "\n";
	for (size_t i = 0; i < servers.size(); i++) {
		if (servers[i].getHost() == responding_server.getHost() && servers[i].getPort() == responding_server.getPort()) {
			if (servers[i].getServerName().find(req_server_name) != req_server_name.npos) {
				responding_server = servers[i];
			}
		}
	}
	// std::cout << responding_server.getServerName();
}

void ft_get_responding_location(std::vector<Location> & locations, Location & responding_location, std::string & req_path) {
	size_t max_match = ft_get_max_match(locations[0].getPath(), req_path);
	size_t tmp;
	size_t idx = 0;
	
	for (size_t i = 0; i < locations.size(); i++) {
		tmp = max_match;
		if ((tmp = ft_get_max_match(locations[i].getPath(), req_path)) > max_match) {
			max_match = tmp;
			idx = i;
		}
	}
	std::cout << "MAX_MATCH " << max_match << "\n";
	if (max_match == 0) {
		for (size_t i = 0; i < locations.size(); i++) {
			if (locations[i].getPath().length() == 1) {
				responding_location = locations[i];
				return ;
			}
		}
	}
	responding_location = locations[idx];
}

void ft_get_location_of_redirected_src(Location & responding_location, Client & client) {
	std::string location;
	(void)client;
	
	if (responding_location.getRedirection().back() == '/')
		responding_location.getRedirection().resize(responding_location.getRedirection().length() - 1);
	if (responding_location.getPath().back() == '/')
		responding_location.getPath().resize(responding_location.getPath().length() - 1);
	location = client.ReqGetPath();
	location.replace(0, responding_location.getPath().length(), responding_location.getRedirection());
	std::cout << "Location " << location << std::endl;
	// exit(1);
};

void ft_replace_req_path(std::string & req_path, std::string & location_path, std::string & location_root) {
	if (location_path.back() == '/') {
		location_path.resize(location_path.length() - 1);
	}
	if (location_root.back() == '/') {
		location_root.resize(location_root.length() - 1);
	}
	req_path.replace(0, location_path.length(), location_root);
	std::cout << "REQ NEW PATH " << req_path << std::endl;
}

size_t ft_get_max_match(std::string & s1, std::string & s2) {
	size_t pos1 = 0, pos2 = 0, max = 0;
	std::string tmp1, tmp2;

	std::cout << "THIS " << s1 << "\n";
	for (; ;) {
		if ((pos1 = s1.find("/", pos1)) == s1.npos)
			return (max);
		if ((pos2 = s2.find("/", pos2)) == s2.npos)
			return (max);
		tmp1 = s1.substr(pos1, s1.find("/", pos1 + 1));
		tmp2 = s2.substr(pos2, s2.find("/", pos2 + 1));
		if (tmp1 != tmp2) {return (max);}
		pos1 += 1; pos2 += 1; max += 1;
	}
	return (max);
}

int ft_get_method(std::string req_method, std::string supported_methods) {
	if (req_method.find("GET") != req_method.npos && supported_methods.find("GET") != supported_methods.npos) {return (1);}
	else if (req_method.find("POST") != req_method.npos && supported_methods.find("POST") != supported_methods.npos) {return (2);}
	else if (req_method.find("DELETE") != req_method.npos && supported_methods.find("DELETE") != supported_methods.npos) {return (3);}
	
	return (-1);
}

int ft_check_protocol(std::string req_protocol) {
	if (req_protocol.find("HTTP/1.1") == req_protocol.npos)
		return (-1);
	return (1);
}

void ft_set_connection_header(Client & client) {
	size_t pos = client.ReqGetConnection().find("close");

	if (pos != client.ReqGetConnection().npos)
		client.RespSetConnection("Connection: close");
	else
		client.RespSetConnection("Connection: keep-alive");
}
