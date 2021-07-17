#include "Header.hpp"
#define BUFFER_SIZE 200

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


void fd_set_reset(fd_set & readfds, fd_set & writefds, int & max_d, \
					int & scoket_fd, std::map<int, Client> & clients) {
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_SET(scoket_fd, &readfds);

	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++) {
		FD_SET(it->first, &readfds);									// it->first is client_fd;
		if (it->second.RespGetRemainedToSent() > 0) {
			FD_SET(it->first, &writefds);
		}
		if (it->first > max_d)
			max_d = it->first;
	}
}

#define TEXT "<html>\nPOKA\n</html>"

void ft_connection_accept(std::map<int, Client> & clients, int & socket_fd) {
	sockaddr_in client_addr;
	socklen_t   client_addr_size;
	int			accept_fd;

	if ((accept_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &client_addr_size)) < 0)
		std::cout << "ACCEPT ERROR\n";
	else std::cout << "ACCEPT OK\n";
	
	if (fcntl(accept_fd, F_SETFL, O_NONBLOCK) < 0)
		std::cout << "FCNTL (ft_connection_accept) ERROR\n";
	else std::cout << "FCNTL (ft_connection_accept) OK\n";
	
	// clients.erase(accept_fd);
	clients[accept_fd];

	ft_create_response(clients, accept_fd);
}

void ft_create_response(std::map<int, Client> & clients, int fd) {
	clients[fd].RespSetProtocol("HTTP/1.1");
	clients[fd].RespSetStatusCode("200");
	clients[fd].RespSetStatusTxt("OK");
	clients[fd].RespSetContentType("text/html");
	clients[fd].RespSetContentLength(strlen(static_cast<const char *>(TEXT)));
	clients[fd].RespSetContent(TEXT);
	
	clients[fd].RespCreateFullRespTxt();
}

void ft_check_clients(std::map<int, Client> & clients, fd_set & readfds, fd_set & writefds) {
	char tmp_buff[BUFFER_SIZE + 1];
	std::string tmp;
	int sent_bytes;
	int ret;

	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++) {
		if (FD_ISSET(it->first, &readfds) && (ret = recv(it->first, &tmp_buff, BUFFER_SIZE, 0))) {
			tmp_buff[ret] = '\0';
			it->second.getBuff().append(tmp_buff);
		}
		if (ret == 0 && it->second.getBuff().size()) {
			// ft_parse_request(clients, it->first, buffer);
			// ft_create_response(clients, it->first);
			std::cout << it->second.getBuff();
			std::cout << "КОНЕЦ ЗАПРОСА \n";
			it->second.getBuff().clear();
		}
		if (FD_ISSET(it->first, &writefds)) {
			tmp = clients[it->first].RespGetFullRespTxt();
			sent_bytes = strlen(clients[it->first].RespGetFullRespTxt().c_str()) - clients[it->first].RespGetRemainedToSent();
			tmp.erase(0, sent_bytes);
			ret = send(it->first, tmp.c_str(), BUFFER_SIZE, 0);
			clients[it->first].RespSetRemainedToSent(clients[it->first].RespGetRemainedToSent() - ret);
		}
	}
}

int main() {
	int         			socket_fd, max_d, res;
	std::map<int, Client>	clients;
	
	fd_set					readfds, writefds;

	socket_fd = ft_socket_init(1);
	while (1) {
		max_d = socket_fd;
		fd_set_reset(readfds, writefds, max_d, socket_fd, clients);
		if ((res = select(max_d + 1, &readfds, &writefds, NULL, NULL)) < 1) {
			if (errno == EINTR) {
                std::cout << "ERROR in SELECT\n";
			}
			else if (res == 0) {
                std::cout << "TIME OUT\n";
			}
			else {
				std::cout << "SIGNAL CAME\n";
			}
		}
		if (FD_ISSET(socket_fd, &readfds)) {
			ft_connection_accept(clients, socket_fd);
		}
		ft_check_clients(clients, readfds, writefds);
	}
	return (0);
}
