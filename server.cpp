#include "Header.hpp"
#define BUFFER_SIZE 1024

typedef struct sockaddr_in sockaddr_in;
int ft_socket_init(int opt);
void fd_set_reset(fd_set & readfds, fd_set & writefds, int & max_d, \
					int & scoket_fd, std::map<int, Client> & clients);
void ft_connection_accept(std::map<int, Client> & clients, int & socket_fd);
void ft_create_response(std::map<int, Client> & clients, int fd);
void ft_check_clients(std::map<int, Client> & clients, fd_set & readfds, fd_set & writefds);


int ft_socket_init(int opt) {
	int         socket_fd;
	sockaddr_in addr;
	
	if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		std::cout << "SOCKET ERROR\n"; 
	else std::cout << "SOCKET OK\n";

	addr.sin_family = PF_INET;
	addr.sin_port = htons(50001);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

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

	if ((kq = kqueue()) == -1)
		std::cout << "KQUEUE ERROR\n";
	else std::cout << "KQUEUE OK\n";

    EV_SET(&chlist[0], socket_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	return (kq);
}

int main() {
	int         			kq, nev;
	unsigned int			socket_fd;
	std::map<int, Client>	clients;

	std::vector<struct kevent> chlist(1);
	std::vector<struct kevent> evlist(1);
	
    struct sockaddr_in client;
    socklen_t client_size = sizeof(client);

	char buf[BUFFER_SIZE];
	
	socket_fd = ft_socket_init(1);
	kq = kqueue_init(chlist, socket_fd);

	const char * text = "HTTP/1.1 200 OK\nContent-Length: 19\nContent-Type: text/html\n\n<html>\nPOKA\n</html>";
    #define TEXT_LEN 79
	while (1) {
		nev = kevent(kq, &chlist.front(), chlist.size(), &evlist.front(), chlist.size(), NULL);
        
		if (nev < 0) {
            std::cout << "kevent ERROR\n";
            exit(EXIT_FAILURE);
        }
        
        else if (nev > 0) {
            std::cout << nev << " kevent OK\n";
            if (evlist[0].flags & EV_EOF) {
                std::cout << "Read direction of socket has shutdown\n";
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i < nev; i++) {
                if (evlist[i].flags & EV_ERROR) {
                    /* Report errors */
                    std::cout << "EV_ERROR\n";
                    exit(EXIT_FAILURE);
                }
                if (evlist[i].ident == socket_fd) {
                    int accept_fd = accept(socket_fd, (struct sockaddr *)&client, &client_size);
                    if (accept_fd < 0) std::cout << "ACCEPT ERROR\n"; else std::cout << "ACCEPT OK\n";
                    std::cout << "FCNTL "  << fcntl(accept_fd, F_SETFL, O_NONBLOCK) << std::endl;

                    struct kevent tmp;
                    EV_SET(&tmp, accept_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
                    chlist.push_back(tmp);
                    evlist.reserve(chlist.size());
                    continue;
                }
                if (evlist[i].filter & EVFILT_READ) {
                     /* We have data from the client */
                        int ret = recv(evlist[i].ident, &buf, BUFSIZ, 0);
                        if (ret < 0) {
                            std::cout << "recv ERROR";
                            exit(1);
                        }
                        buf[ret] = '\0';
                        std::cout << buf;
                    }
                if (evlist[i].filter & EVFILT_WRITE) {
                    // if (у нас что-то есть для отправки)
                        int ret = send(evlist[i].ident, text, TEXT_LEN, 0);
                        if (ret < 0) std::cout << "SEND ERROR\n"; else  std::cout << "SEND OK\n";
                }
            }
        }

	}
	return (0);
}
