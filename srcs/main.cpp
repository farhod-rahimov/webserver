#include "./headers/Header.hpp"
#define BUFFER_SIZE 100000 // get - макс 2048 байт

std::vector<Server>	servers;

int ft_socket_init(Server & server, int opt) {
	int         socket_fd;
	sockaddr_in addr;
	
	for (size_t i = 0; i < servers.size(); i++) {
		if (server.getHost() == servers[i].getHost() && server.getPort() == servers[i].getPort()) {
			if (&server != &servers[i] && server.getCheckFlag() && servers[i].getCheckFlag()) {
				servers[i].getCheckFlag() = false; return (-1);
			}
		}
	}
 
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		std::cout << "SOCKET ERROR\n"; 
	else std::cout << "SOCKET OK\n";

	addr.sin_family = PF_INET;
	
	addr.sin_port = htons(std::atoi(server.getPort().c_str()));
	addr.sin_addr.s_addr = inet_addr(server.getHost().c_str());

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		std::cout << "SET_SOCK_OPT ERROR";
	
	if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		std::cout << "BIND ERROR\n";
	else std::cout << "BIND OK\n";
	
	if (listen(socket_fd, 50) < 0)
		std::cout << "LISTEN ERROR\n";
	else std::cout << "LISTEN OK\n";

	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0)
		std::cout << "FCNTL (ft_socket_init) ERROR\n";
	else std::cout << "FCNTL (ft_socket_init) OK\n";
	
	return (socket_fd);
}

int kqueue_init(void) {
	int kq;

	if ((kq = kqueue()) == -1)
		std::cout << "KQUEUE ERROR\n";
	else std::cout << "KQUEUE OK\n";

	return (kq);
}

bool ft_check_evlist_error(std::vector<struct kevent> & chlist, std::vector<struct kevent> & evlist) {
	size_t i;

	if (evlist[0].flags & EV_EOF) {
		for (i = 0; i < chlist.size(); i++) {
			if (chlist[i].ident == evlist[0].ident) {
				std::cout <<  chlist[i].ident << " " << evlist[0].ident << " Read direction of socket has shutdown\n";
				break ;
			}
			else if (i + 1 == chlist.size()) {
				std::cout <<  chlist[i].ident << " " << evlist[0].ident << " NNN Read direction of socket has shutdown\n";
			}
		}
		close(chlist[i].ident);
		chlist.erase(chlist.begin() + i);
		return (true);
	}
	return (false);
}

bool ft_check_new_connection(int & socket_fd, int & i, std::vector<struct kevent> & chlist, \
								std::vector<struct kevent> & evlist, std::map<int, Client> & clients) {
	struct sockaddr_in client;
    socklen_t client_size = sizeof(client);
	
	if (evlist[i].ident == static_cast<unsigned int>(socket_fd)) {
		int accept_fd = accept(socket_fd, (struct sockaddr *)&client, &client_size);
		if (accept_fd < 0) std::cout << "ACCEPT ERROR\n"; else std::cout << "ACCEPT OK\n";
		std::cout << "FCNTL "  << fcntl(accept_fd, F_SETFL, O_NONBLOCK) << std::endl;

		struct kevent tmp;
		chlist.push_back(tmp);
		EV_SET(&chlist.back(), accept_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
		evlist.reserve(chlist.size());
		
		clients[accept_fd];
		return (true);
	}
	return (false);
}

void ft_check_clients(int & i, std::vector<struct kevent> & chlist, std::vector<struct kevent> & evlist, Server & server) {
	char buf[BUFFER_SIZE + 1];
	int ret = BUFFER_SIZE + 1;
	
	std::map<int, Client> & clients = server.getClients();
    (void)chlist;
    int fd = evlist[i].ident;
	if (evlist[i].filter & EVFILT_READ) {
			ret = recv(fd, &buf, BUFFER_SIZE, 0);
			if (ret < 0) {
				std::cout << "recv ERROR";
				return ;
				// exit(EXIT_FAILURE);
			}
			buf[ret] = '\0';
			for (int i = 0; i < ret; i++) {
				clients[fd].getBuff().push_back(buf[i]);
			}
			// std::cout << ret << " CAME REQUEST\n'" << clients[fd].getBuff() << "'\n";
    }
	if (ft_check_end_request(clients[fd].getBuff()) == true){

    // if (ret < BUFFER_SIZE) {
    // if (clients[fd].getBuff().size() && clients[fd].getBuff().find("\r\n\r\n") != clients[fd].getBuff().npos) {
                    /* наличие сообщения в запросе не учтено, если оно есть то нужно сюда заходить после получения */
        ft_parse_request(clients, fd);
        ft_send_response(server, fd, chlist, servers);
        clients[fd].getBuff().clear();
    }
}

void ft_check_fds(int & nev, int & socket_fd, std::vector<struct kevent> & chlist, \
					std::vector<struct kevent> & evlist, Server & server) {
	std::map<int, Client> & clients = server.getClients();
	
	for (int i = 0; i < nev; i++) {
		if (evlist[i].flags & EV_ERROR) { /* Report errors */
			std::cout << "EV_ERROR\n";
			continue ;
			// exit(EXIT_FAILURE);
		}
		if (ft_check_new_connection(socket_fd, i, chlist, evlist, clients) == true) {
			continue ;
		}
		ft_check_clients(i, chlist, evlist, server);
	}
};


int main(int argc, char **argv) {
	int					kq, nev;
	
	if (argc == 1) {ft_parse(servers, DEFAULT_CONF);}
	else {ft_parse(servers, argv[1]);}
	
	std::vector<int>	socket_fd;
	std::vector<struct kevent>	chlist, evlist;

	kq = kqueue_init();
	struct kevent tmp_kev;
	int tmp_sock;
	for (size_t i = 0; i < servers.size(); i++) {
		if ((tmp_sock = ft_socket_init(servers[i], 1)) > 0) {
			socket_fd.push_back(tmp_sock);
			chlist.push_back(tmp_kev);
			evlist.push_back(tmp_kev);
    		EV_SET(&chlist.back(), socket_fd.back(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
		}
	}
	
	while (1) {
		nev = kevent(kq, &chlist.front(), chlist.size(), &evlist.front(), chlist.size(), NULL);
		
		if (nev < 0) {
			std::cout << "kevent ERROR\n"; exit(EXIT_FAILURE);
		}
		else if (nev > 0) {
			// std::cout << nev << " kevent OK\n";
			if (ft_check_evlist_error(chlist, evlist)) {
				continue ;
			}
			int n = 0;
			for (size_t i = 0; i < servers.size(); i++) {
				if (servers[i].getCheckFlag()) {
					ft_check_fds(nev, socket_fd[n++], chlist, evlist, servers[i]);
					// ft_check_fds(nev, socket_fd[n++], chlist, evlist, servers[i].getClients());
				}
			}
		}
	}
	return (0);
}
