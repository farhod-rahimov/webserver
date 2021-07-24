#ifndef HEADER_HPP
# define HEADER_HPP

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

# include <sys/types.h>    //kevent
# include <sys/event.h>
# include <sys/time.h>

# include <fstream>
# include <string.h>

# include "Client.hpp"
# include "Server.hpp"

# define DEFAULT_CONF "/Users/keuclide/cursus/main_webserver/hosted_website/config/default.conf"

typedef struct sockaddr_in sockaddr_in;

int		ft_socket_init(Server & server, int opt);
int		kqueue_init(void);
bool	ft_check_evlist_error(std::vector<struct kevent> & chlist, std::vector<struct kevent> & evlist);


bool	ft_check_new_connection(unsigned int & socket_fd, int & i, std::vector<struct kevent> & chlist, \
								std::vector<struct kevent> & evlist, std::map<int, Client> & clients);

void	ft_parse_request(std::map<int, Client> & clients, int fd);

void    ft_send_response(std::map<int, Client> & clients, size_t fd, std::vector<struct kevent> & chlist);
void    ft_create_response(Client & client);

void	ft_check_clients(int & i, std::vector<struct kevent> & chlist, \
						std::vector<struct kevent> & evlist, std::map<int, Client> & clients);

void	ft_check_fds(int & nev, unsigned int & socket_fd, std::vector<struct kevent> & chlist, \
					std::vector<struct kevent> & evlist, std::map<int, Client> & clients);

void	ft_response_to_get(Client & client);
void	ft_response_to_post(Client & client);
void	ft_response_to_delete(Client & client);
void	ft_create_my_def_response(Client & client);

int		ft_read_file(const char * filename,  std::string & content);
void	ft_send_not_found(Client & client);

void	ft_create_header(const char * content_type, Client & client, std::string & content);

void	ft_parse(std::vector<Server> & servers, const char *conf_file);

#endif