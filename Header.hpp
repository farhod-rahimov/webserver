#ifndef HEADER_HPP
# define HEADER_HPP

#include <sys/socket.h>  // socket
#include <sys/types.h>   // connect
#include <iostream>
#include <netinet/in.h>  // struct sockaddr_in

#include <arpa/inet.h>   // inet_addr
#include <sys/select.h>  // select
#include <fcntl.h>

#include <vector>
#include <map>
#include <unistd.h>

#include <sys/types.h>    //kevent
#include <sys/event.h>
#include <sys/time.h>

#include "./srcs/Client.hpp"


typedef struct sockaddr_in sockaddr_in;

int		ft_socket_init(int opt);
int		kqueue_init(std::vector<struct kevent> & chlist, int socket_fd);
bool	ft_check_evlist_error(std::vector<struct kevent> & chlist, std::vector<struct kevent> & evlist);


bool	ft_check_new_connection(unsigned int & socket_fd, int & i, std::vector<struct kevent> & chlist, \
								std::vector<struct kevent> & evlist, std::map<int, Client> & clients);

void	ft_parse_request(std::map<int, Client> & clients, int fd);

void    ft_send_response(std::map<int, Client> & clients, int fd);
void    ft_create_response(Client & client);

void	ft_check_clients(int & i, std::vector<struct kevent> & chlist, \
						std::vector<struct kevent> & evlist, std::map<int, Client> & clients);

void	ft_check_fds(int & nev, unsigned int & socket_fd, std::vector<struct kevent> & chlist, \
					std::vector<struct kevent> & evlist, std::map<int, Client> & clients);

#endif