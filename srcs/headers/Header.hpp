#ifndef HEADER_HPP
# define HEADER_HPP

#define BUFFER_SIZE 100000 // get - макс 2048 байт
#define CLRF "\r\n"
#define DOUBLE_CLRF "\r\n\r\n"


#include <sys/socket.h>  // socket
#include <sys/types.h>   // connect
#include <iostream>
#include <netinet/in.h>  // struct sockaddr_in

#include <arpa/inet.h>   // inet_addr
#include <sys/select.h>  // select
#include <fcntl.h>

# include <sys/socket.h>  // socket
# include <sys/types.h>   // connect
# include <iostream>
# include <netinet/in.h>  // struct sockaddr_in

# include <arpa/inet.h>   // inet_addr
# include <sys/select.h>  // select
# include <fcntl.h>

# include <vector>
# include <map>
# include <unistd.h>
#include <vector>
#include <string>
#include <map>
#include <unistd.h>

#include <sys/types.h>    //kevent
#include <sys/event.h>
#include <sys/time.h>

# include <fstream>
# include <string.h>
#include <fstream>
#include <sstream>

#include "Client.hpp"
#include "Server.hpp"

#include "Cgi.hpp"

// #define DEFAULT_CONF "/Users/keuclide/cursus/main_webserver/hosted_website/config/default.conf"

#define DEFAULT_CONF "/Users/btammara/webserver/default_works.conf"
// #define DEFAULT_CONF "/Users/btammara/webserver/hosted_website/config/default.conf"

// std::vector<Server>	servers;

typedef struct sockaddr_in sockaddr_in;

int		ft_socket_init(Server & server, int opt);
int		kqueue_init(void);
bool	ft_check_evlist_error(std::vector<struct kevent> & chlist, std::vector<struct kevent> & evlist);


bool	ft_check_new_connection(int & socket_fd, int & i, std::vector<struct kevent> & chlist, \
								std::vector<struct kevent> & evlist, std::map<int, Client> & clients);

void	ft_parse_request(std::map<int, Client> & clients, int fd);
void	ft_parse_request(std::map<int, Client> & clients, int fd);

void    ft_send_response(Server & server, size_t fd, std::vector<struct kevent> & chlist, std::vector<Server> & servers, int kq);
void    ft_create_response(Client & client, std::vector<Server> & servers, Server responding_server, int fd);

void	ft_check_clients(int & i, std::vector<struct kevent> & chlist, \
						std::vector<struct kevent> & evlist, Server & server);

void	ft_check_fds(int & nev, int & socket_fd, std::vector<struct kevent> & chlist, \
					std::vector<struct kevent> & evlist, Server & server);

// void	ft_response_to_get(Client & client);
void	ft_response_to_get(Client & client, Server & server, Location & location, int fd);

// void	ft_response_to_post(Client & client);
void	ft_response_to_post(Client & client, Server & server, Location & location, int fd);

// void	ft_response_to_delete(Client & client);
void	ft_response_to_delete(Client & client, Server & server, Location & location, int fd);

void	ft_create_my_def_response(Client & client);

int		ft_read_file(const char * filename,  std::string & content);
void	ft_send_not_found(Client & client);

void	ft_create_header(const char * content_type, Client & client, std::string & content);

void	ft_parse(std::vector<Server> & servers, const char *conf_file);

bool	ft_check_end_request(std::string & buf);

void	ft_send_not_implemented(Client & client);

std::string ft_get_req_path_extension(Client & client);
void ft_work_with_cgi(Client & client, Server & server, Location & location, int fd);

void ft_send_too_long_body(Client & client, int fd, int k);

void ft_send_ok(Client & client);
void ft_get_responding_server(std::vector<Server> & servers, Client & client, Server & responding_server);


void ft_create_new_write_event(int kq, int fd);

/////trash.cpp
void ft_print_result(std::vector<Server> & servers);

// std:cout change to std::cerr

#endif
