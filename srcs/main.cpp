#include "./headers/Header.hpp"

std::vector<Server>	servers;
int	kq;

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
 
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "SOCKET	ERROR\n"; exit(EXIT_FAILURE);
	}
	else std::cout << "SOCKET	OK\n";

	addr.sin_family = PF_INET;
	addr.sin_port = htons(std::atoi(server.getPort().c_str()));
	addr.sin_addr.s_addr = inet_addr(server.getHost().c_str());

	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "SET_SOCK_OPT	ERROR"; exit(EXIT_FAILURE);
	}
	
	if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		std::cerr << "BIND	ERROR\n"; exit(EXIT_FAILURE);
	}
	else std::cout << "BIND	OK\n";
	
	if (listen(socket_fd, 50) < 0) {
		std::cerr << "LISTEN	ERROR\n"; exit(EXIT_FAILURE);
	}
	else std::cout << "LISTEN	OK\n";

	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << "FCNTL (ft_socket_init)	ERROR\n"; exit(EXIT_FAILURE);
	}
	// else std::cout << "FCNTL (ft_socket_init)	OK\n";
	
	return (socket_fd);
}

int kqueue_init(void) {
	if ((kq = kqueue()) == -1) {
		std::cerr << "KQUEUE	ERROR\n"; exit(EXIT_FAILURE);
	}
	else std::cout << "KQUEUE	OK\n";

	return (kq);
}

bool ft_check_evlist_error(std::vector<struct kevent> & chlist, std::vector<struct kevent> & evlist) {
	size_t i;

	if (evlist[0].flags & EV_EOF) {
		for (i = 0; i < chlist.size(); i++) {
			if (chlist[i].ident == evlist[0].ident) {
				std::cout << "CLIENT " << evlist[0].ident << " CLOSED CONNECTION\n";
				break ;
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
	struct sockaddr_in	client;
    socklen_t			client_size = sizeof(client);
	struct kevent		tmp;
	
	if (evlist[i].ident == static_cast<unsigned int>(socket_fd)) {
		int accept_fd = accept(socket_fd, (struct sockaddr *)&client, &client_size);
		if (accept_fd < 0) {
			std::cerr << "ACCEPT	ERROR\n";
			return (false);
		}
		else std::cout << "ACCEPT	OK\n";

		if (fcntl(accept_fd, F_SETFL, O_NONBLOCK) < 0) {
			std::cerr << "FCNTL (ft_check_new_connection)	ERROR\n";
			close(accept_fd);
			return (false);
		}
		// else std::cout << "FCNTL (ft_check_new_connection)	OK\n";

		chlist.push_back(tmp);
		EV_SET(&chlist.back(), accept_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
		evlist.reserve(chlist.size());
		
		clients[accept_fd];
		return (true);
	}
	return (false);
}

void ft_check_clients(int & i, std::vector<struct kevent> & chlist, std::vector<struct kevent> & evlist, Server & server) {
	std::map<int, Client> &	clients = server.getClients();
	int						fd = evlist[i].ident;
	char					buf[BUFFER_SIZE + 1];
	int						ret;
	
	if (evlist[i].filter == EVFILT_READ) {
		bzero(buf, BUFFER_SIZE + 1);
		if ((ret = recv(fd, &buf, BUFFER_SIZE, 0)) < 0) {
			std::cerr << "RECV	ERROR";
			ft_remove_client(chlist, fd);
			return ;
		}
		buf[ret] = '\0';
		for (int i = 0; i < ret; i++) {
			clients[fd].getBuff().push_back(buf[i]);
		}
		if (ft_check_end_request(clients[fd].getBuff()) == true) {
			ft_create_new_write_event(kq, fd);
	        ft_parse_request(clients, fd);
			
			if (clients[fd].ReqGetContentLength() > static_cast<size_t>(std::atoi(server.getLimitBodySize().c_str()))) {
				ft_send_too_long_body(clients[fd]);
			}
			else {
				ft_create_response(clients[fd], servers, server, fd);
			}
			clients[fd].getBuff().clear();
			clients[fd].ReqSetContentLength(0);
		}
    }
	if (evlist[i].filter == EVFILT_WRITE) {
        ft_send_response(server, fd, chlist, kq);
    }
}

void ft_check_fds(int & nev, int & socket_fd, std::vector<struct kevent> & chlist, \
					std::vector<struct kevent> & evlist, Server & server) {
	
	std::map<int, Client> &			clients = server.getClients();
	std::map<int, Client>::iterator	it = clients.begin();
	int								i;

	for (i = 0; i < nev; i++) {
		if (evlist[i].flags & EV_ERROR) {
			continue ;
		}
		if (ft_check_new_connection(socket_fd, i, chlist, evlist, clients) == true) {
			continue ;
		}
	}

	for (i = 0; ; ) {
		if (it != clients.end() && static_cast<size_t>(it->first) == evlist[i].ident) {
			if (!(evlist[i].flags & EV_ERROR)) {
				ft_check_clients(i, chlist, evlist, server);
			}
		}
		if (it == clients.end() && ++i < nev) it = clients.begin();
		else if (it == clients.end()) break ;
		else it++;
	}
};

int main(int argc, char **argv) {
	std::vector<int>			socket_fd;
	std::vector<struct kevent>	chlist, evlist;
	struct kevent				tmp_kev;
	int							nev, tmp_sock;
	
	if (argc == 1) {ft_parse(servers, DEFAULT_CONF);}
	else {ft_parse(servers, argv[1]);}

	kq = kqueue_init();
	for (size_t i = 0; i < servers.size(); i++) {
		if ((tmp_sock = ft_socket_init(servers[i], 1)) > 0) {
			socket_fd.push_back(tmp_sock);
			chlist.push_back(tmp_kev);
			evlist.push_back(tmp_kev);
    		EV_SET(&chlist.back(), socket_fd.back(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
		}
	}
	
	while (1) {
		evlist.reserve(chlist.size() * 2);
		nev = kevent(kq, &chlist.front(), chlist.size(), &evlist.front(), evlist.capacity(), NULL);
		if (nev < 0) {
			std::cerr << "kevent	ERROR\n"; exit(EXIT_FAILURE);
		}
		else if (nev > 0) {
			if (ft_check_evlist_error(chlist, evlist)) {
				continue ;
			}
			int n = 0;
			for (size_t i = 0; i < servers.size(); i++) {
				if (servers[i].getCheckFlag()) {
					ft_check_fds(nev, socket_fd[n++], chlist, evlist, servers[i]);
				}
			}
		}
	}
	return (0);
}
