#include "./headers/Header.hpp"

static void ft_get_responding_server(std::vector<Server> & servers, Client & client, Server & responding_server);
static void ft_get_responding_location(std::vector<Location> & locations, Location & responding_location, std::string & req_path);
static size_t ft_get_max_match(std::string & s1, std::string & s2);
static void ft_replace_req_path(std::string & req_path, std::string & location_path, std::string & location_root);
static void ft_get_location_of_redirected_src(Location & responding_location, Client & client);

static int ft_get_method(std::string & req_method, std::string & supported_methods);
static int ft_check_protocol(std::string & req_protocol);
static void ft_set_connection_header(Client & client);

void ft_send_response(Server & server, size_t fd, std::vector<struct kevent> & chlist, int kq) {
	std::map<int, Client> & clients = server.getClients();
	int already_sent 				= clients[fd].RespGetFullRespTxt().length() - clients[fd].RespGetRemainedToSent();
	int ret = 0; size_t i = 0;

	if (clients[fd].RespGetRemainedToSent() > 0) {
		if ((ret = send(fd, clients[fd].RespGetFullRespTxt().c_str() + already_sent, clients[fd].RespGetRemainedToSent(), 0)) < 0) {
			std::cerr << "Error. Cannot send response\n";
			ft_send_internal_error(clients[fd]); return ;
		}
	}

	clients[fd].RespSetRemainedToSent(clients[fd].RespGetRemainedToSent() - ret);
	std::cout << "TOTAL BYTES TO SEND		" << ret << std::endl;
	std::cout << "REMAINED BYTES TO SENT	" << clients[fd].RespGetRemainedToSent() << std::endl;
	
	if (clients[fd].RespGetRemainedToSent() <= 0) {
		struct kevent tmp; EV_SET(&tmp, fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
		kevent(kq, &tmp, 1, NULL, 0, NULL);
		if (clients[fd].RespGetConnection().find("close") != clients[fd].RespGetConnection().npos) {
			for (i = 0; i < chlist.size() && chlist[i].ident != fd; i++) {}
			close(fd); chlist.erase(chlist.begin() + i);
		}
	}
}

void ft_create_response(Client & client, std::vector<Server> & servers, Server responding_server, int fd) {
	Location responding_location;
	int method;
	
	ft_get_responding_server(servers, client, responding_server);
	ft_get_responding_location(responding_server.getLocations(), responding_location, client.ReqGetPath());
	
	if (responding_location.getRedirection().empty())
		ft_replace_req_path(client.ReqGetPath(), responding_location.getPath(), responding_location.getLocationRoot());
	else
		ft_get_location_of_redirected_src(responding_location, client);
	
	ft_set_connection_header(client);

	if ((method = ft_get_method(client.ReqGetMethod(), responding_location.getAllowedMethods())) < 0)
		return (ft_send_not_implemented(client));
	if (ft_check_protocol(client.ReqGetProtocol()) < 0)
		return (ft_send_protocol_not_supported(client));
	
	if (method == 1) {ft_response_to_get(client, responding_server, responding_location, fd);}
	else if (method == 2) {ft_response_to_post(client, responding_server, responding_location, fd);}
	else if (method == 3) {ft_response_to_delete(client);}
	else ft_send_method_not_allowed(client);
}

static void ft_get_responding_server(std::vector<Server> & servers, Client & client, Server & responding_server) {
	std::string req_server_host, req_server_port;

	if (client.ReqGetHost().find(":") != client.ReqGetHost().npos) {
		req_server_host = client.ReqGetHost().substr(0, client.ReqGetHost().find(":"));
		req_server_port = client.ReqGetHost().substr(client.ReqGetHost().find(":") + 1);

		for (size_t i = 0; i < servers.size(); i++) {
			if (servers[i].getServerName().find(req_server_host) != std::string::npos && req_server_port == servers[i].getPort()) {
				responding_server = servers[i];
				return ;
			}
		}
		for (size_t i = 0; i < servers.size(); i++) {
			if (req_server_host == servers[i].getHost() && req_server_port == servers[i].getPort()) {
				responding_server = servers[i];
				return ;
			}
		}
	}
	for (size_t i = 0; i < servers.size(); i++) {
		for (size_t n = 0; n < servers[i].getClients().size(); n++) {
			if (&servers[i].getClients()[n] == &client) {
				responding_server = servers[i];
				return ;
			}
		}
	}
}

static void ft_get_responding_location(std::vector<Location> & locations, Location & responding_location, std::string & req_path) {
	size_t max_match = ft_get_max_match(locations[0].getPath(), req_path);
	size_t tmp, idx = 0;
	
	for (size_t i = 0; i < locations.size(); i++) {
		tmp = max_match;
		if ((tmp = ft_get_max_match(locations[i].getPath(), req_path)) > max_match) {
			max_match = tmp;
			idx = i;
		}
	}
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

static void ft_get_location_of_redirected_src(Location & responding_location, Client & client) {
	if (responding_location.getRedirection().back() == '/')
		responding_location.getRedirection().pop_back();
	if (responding_location.getPath().back() == '/')
		responding_location.getPath().pop_back();
	
	client.RespGetLocation() = client.ReqGetPath();
	client.RespGetLocation().replace(0, responding_location.getPath().length(), responding_location.getRedirection());
	client.RespSetLocation(client.RespGetLocation());
};

static void ft_replace_req_path(std::string & req_path, std::string & location_path, std::string & location_root) {
	if (location_path.back() == '/')
		location_path.pop_back();
	if (location_root.back() == '/')
		location_root.pop_back();
	req_path.replace(0, location_path.length(), location_root);
}

static size_t ft_get_max_match(std::string & s1, std::string & s2) {
	size_t pos1 = 0, pos2 = 0, max = 0;
	std::string tmp1, tmp2;

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

static int ft_get_method(std::string & req_method, std::string & supported_methods) {
	if (req_method.find("GET") != req_method.npos && supported_methods.find("GET") != supported_methods.npos) {return (1);}
	else if (req_method.find("POST") != req_method.npos && supported_methods.find("POST") != supported_methods.npos) {return (2);}
	else if (req_method.find("DELETE") != req_method.npos && supported_methods.find("DELETE") != supported_methods.npos) {return (3);}
	
	return (-1);
}

static int ft_check_protocol(std::string & req_protocol) {
	if (req_protocol.find("HTTP/1.1") == req_protocol.npos)
		return (-1);
	return (1);
}

static void ft_set_connection_header(Client & client) {
	size_t pos = client.ReqGetConnection().find("close");

	if (pos != client.ReqGetConnection().npos)
		client.RespSetConnection("Connection: close");
	else
		client.RespSetConnection("Connection: keep-alive");
}
