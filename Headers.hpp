#ifndef HEADERS_HPP
# define HEADERS_HPP

#include <sys/socket.h>  // socket
#include <sys/types.h>   // connect
#include <iostream>
#include <netinet/in.h>  // struct sockaddr_in

#include <arpa/inet.h>   // inet_addr
#include <sys/select.h>  // select
#include <fcntl.h>

#include <vector>
#include <unistd.h>

#include "./srcs/Client.hpp"
#include "./srcs/Response.hpp"
#include "./srcs/Request.hpp"


#endif