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

#endif