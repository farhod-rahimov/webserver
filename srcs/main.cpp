#include "./headers/Header.hpp"
#define BUFFER_SIZE 1000 // get - макс 2048 байт

int ft_socket_init(Server & server, int opt) {
	int         socket_fd;
	sockaddr_in addr;
	
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		std::cout << "SOCKET ERROR\n"; 
	else std::cout << "SOCKET OK\n";

	addr.sin_family = PF_INET;
	
	addr.sin_port = htons(std::atoi(server.getPort().c_str()));
	addr.sin_addr.s_addr = inet_addr(server.getHost().c_str());

	// addr.sin_port = htons(50001);
	// addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

int kqueue_init(std::vector<struct kevent> & chlist, int socket_fd) {
	int kq;
	(void)chlist;
	(void)socket_fd;

	if ((kq = kqueue()) == -1)
		std::cout << "KQUEUE ERROR\n";
	else std::cout << "KQUEUE OK\n";

    EV_SET(&chlist[0], socket_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
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
        // evlist.clear();
        // evlist.reserve(chlist.size());
		// std::cout <<  chlist[i].ident << " " << evlist[0].ident << " Read direction of socket has shutdown\n";
		return (true);
	}
	return (false);
}

bool ft_check_new_connection(unsigned int & socket_fd, int & i, std::vector<struct kevent> & chlist, \
								std::vector<struct kevent> & evlist, std::map<int, Client> & clients) {
	struct sockaddr_in client;
    socklen_t client_size = sizeof(client);
	
	if (evlist[i].ident == socket_fd) {
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

void ft_check_clients(int & i, std::vector<struct kevent> & chlist, std::vector<struct kevent> & evlist, std::map<int, Client> & clients) {
	char buf[BUFFER_SIZE + 1];
	int ret;

    (void)chlist;
    int fd = evlist[i].ident;
	if (evlist[i].filter & EVFILT_READ) {
			/* We have data from the client */
			ret = recv(fd, &buf, BUFFER_SIZE, 0);
			if (ret < 0) {
				std::cout << "recv ERROR";
				return ;
				// exit(EXIT_FAILURE);
			}
			buf[ret] = '\0';
            clients[fd].getBuff().append(buf);
    }
    if (clients[fd].getBuff().size() && clients[fd].getBuff().find("\r\n\r\n") != clients[fd].getBuff().npos) {
                    /* наличие сообщения в запросе не учтено, если оно есть то нужно сюда заходить после получения */
        ft_parse_request(clients, fd);
        ft_send_response(clients, fd, chlist);
        clients[fd].getBuff().clear();
    }
}

void ft_check_fds(int & nev, unsigned int & socket_fd, std::vector<struct kevent> & chlist, \
					std::vector<struct kevent> & evlist, std::map<int, Client> & clients) {
	for (int i = 0; i < nev; i++) {
		if (evlist[i].flags & EV_ERROR) { /* Report errors */
			std::cout << "EV_ERROR\n";
			continue ;
			// exit(EXIT_FAILURE);
		}
		if (ft_check_new_connection(socket_fd, i, chlist, evlist, clients) == true) {
			continue ;
		}
		ft_check_clients(i, chlist, evlist, clients);
	}
};


int main(int argc, char **argv) {
	// int         				kq, nev;
	// unsigned int				socket_fd;
	// std::vector<struct kevent>	chlist(1);
	// std::vector<struct kevent>	evlist(1);
	// std::vector<Server>			servers;

	int											nev;
	std::vector<Server>							servers;
	
	// std::vector<Server>			servers(1);

	if (argc == 1) {ft_parse(servers, DEFAULT_CONF);}
	else {ft_parse(servers, argv[1]);}
	
	std::vector<int>							kq(servers.size());
	std::vector<unsigned int>					socket_fd(servers.size());
	std::vector<std::vector<struct kevent> > chlist(servers.size()), evlist(servers.size());

	struct  kevent empty;

	for (size_t i = 0; i < servers.size(); i++) {
		chlist[i].push_back(empty);
		evlist[i].push_back(empty);
	
		socket_fd[i] = ft_socket_init(servers[i], 1);
		kq[i] = kqueue_init(chlist[i], socket_fd[i]);
	}
	
	// socket_fd = ft_socket_init(1);
	// kq = kqueue_init(chlist, socket_fd);

	// std::cout << "SER SIZE " << servers.size();
	// exit(1);

	while (1) {
		for (size_t i = 0; i < servers.size(); i++) {
			std::cout << "																i = " << i << std::endl;
			nev = kevent(kq[i], &chlist[i].front(), chlist[i].size(), &evlist[i].front(), chlist[i].size(), NULL);
			
			if (nev < 0) {
				std::cout << "kevent ERROR\n"; exit(EXIT_FAILURE);
			}
			else if (nev > 0) {
				// std::cout << nev << " kevent OK\n";
				if (ft_check_evlist_error(chlist[i], evlist[i])) {
					continue ;
				}
				ft_check_fds(nev, socket_fd[i], chlist[i], evlist[i], servers[i].getClients());
			}
		}
	}
	return (0);
}

// int main(int argc, char **argv) {
// 	int         				kq, nev;
// 	unsigned int				socket_fd;
// 	std::vector<struct kevent>	chlist(1);
// 	std::vector<struct kevent>	evlist(1);
// 	std::vector<Server>			servers;
	
// 	if (argc == 1) {
// 		ft_parse(servers, DEFAULT_CONF);
// 	}
// 	else
// 		ft_parse(servers, argv[1]);
	
// 	socket_fd = ft_socket_init(servers[0], 1);
// 	kq = kqueue_init(chlist, socket_fd);

// 	while (1) {
//         nev = kevent(kq, &chlist.front(), chlist.size(), &evlist.front(), chlist.size(), NULL);
        
// 		if (nev < 0) {
//             std::cout << "kevent ERROR\n"; exit(EXIT_FAILURE);
//         }
//         else if (nev > 0) {
// 			// std::cout << nev << " kevent OK\n";
// 			if (ft_check_evlist_error(chlist, evlist)) {
// 				continue ;
// 			}
// 			ft_check_fds(nev, socket_fd, chlist, evlist, servers[0].getClients());
//         }
// 	}
// 	return (0);
// }
