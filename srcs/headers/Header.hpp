#ifndef HEADER_HPP
# define HEADER_HPP

#include <sys/socket.h> 
#include <sys/types.h>
#include <sys/select.h>
#include <sys/event.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <iostream>
#include <sstream>
#include <fstream>

#include <map>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>

#include "Client.hpp"
#include "Server.hpp"
#include "Cgi.hpp"

#define BUFFER_SIZE 100000
#define CLRF "\r\n"
#define DOUBLE_CLRF "\r\n\r\n"

#define DEFAULT_CONF "/Users/btammara/webserver/default_works.conf"

typedef struct sockaddr_in sockaddr_in;

int			ft_socket_init(Server & server, int opt);
int			kqueue_init(void);
bool		ft_check_evlist_error(std::vector<struct kevent> & chlist, std::vector<struct kevent> & evlist);
bool		ft_check_new_connection(int & socket_fd, int & i, std::vector<struct kevent> & chlist, \
									std::vector<struct kevent> & evlist, std::map<int, Client> & clients);

void		ft_parse_request(std::map<int, Client> & clients, int fd);
void		ft_send_response(Server & server, size_t fd, std::vector<struct kevent> & chlist, int kq);
void    	ft_create_response(Client & client, std::vector<Server> & servers, Server responding_server, int fd);
void		ft_check_clients(int & i, std::vector<struct kevent> & chlist, \
							std::vector<struct kevent> & evlist, Server & server);
void		ft_check_fds(int & nev, int & socket_fd, std::vector<struct kevent> & chlist, \
						std::vector<struct kevent> & evlist, Server & server);
void		ft_response_to_get(Client & client, Server & server, Location & location, int fd);
void		ft_response_to_post(Client & client, Server & server, Location & location, int fd);
void		ft_response_to_delete(Client & client);
int			ft_read_file(const char * filename,  std::string & content);
void		ft_send_not_found(Client & client);
void		ft_create_header(const char * content_type, Client & client, std::string & content);
void		ft_parse(std::vector<Server> & servers, const char *conf_file);
bool		ft_check_end_request(std::string & buf);
void		ft_send_not_implemented(Client & client);
void 		ft_send_204_not_content(Client & client);
void 		ft_send_file_was_created(Client & client);
void 		ft_send_protocol_not_supported(Client & client);
void 		ft_send_method_not_allowed(Client & client);
std::string ft_get_req_path_extension(Client & client);
int 		ft_work_with_cgi(Client & client, Server & server, Location & location, int fd);
std::string ft_get_req_path_extension(Client & client);
void 		ft_send_too_long_body(Client & client, int fd, int k);
void 		ft_send_internal_error(Client & client);
int 		ft_show_current_dir_files(Client & client, Location & location, std::string & content);
void 		ft_send_forbidden(Client & client);
void 		ft_create_new_write_event(int kq, int fd);

/////trash.cpp
void ft_print_result(std::vector<Server> & servers);

#endif
