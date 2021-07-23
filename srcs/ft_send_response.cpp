#include "./headers/Header.hpp"

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

void ft_create_response(Client & client, std::vector<Server> & servers, Server responding_server) {

	ft_get_responding_server(servers, client, responding_server);

	exit(1);
}

void ft_get_responding_server(std::vector<Server> & servers, Client & client, Server & responding_server) {
	
	std::string req_server_name;

	if (client.ReqGetHost().find(":") != client.ReqGetHost().npos)
		req_server_name = client.ReqGetHost().substr(0, client.ReqGetHost().find(":"));
	else
		req_server_name = client.ReqGetHost();

	std::cout << "req_host " << req_server_name << "\n";
	
	for (size_t i = 0; i < servers.size(); i++) {
		if (servers[i].getHost() == responding_server.getHost() && servers[i].getPort() == responding_server.getPort()) {
			if (servers[i].getServerName().find(req_server_name) != req_server_name.npos) {
				responding_server = servers[i];
			}
		}
	}
	std::cout << responding_server.getServerName();
}

