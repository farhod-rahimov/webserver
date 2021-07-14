#include <sys/socket.h>  // socket
#include <sys/types.h>   // connect
#include <iostream>
#include <netinet/in.h>  // struct sockaddr_in

#include <arpa/inet.h>   // inet_addr

int main() {
    int socket_fd;
    int ret;
    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) std::cout << "SOCKET ERROR\n"; else std::cout << "SOCKET OK\n";

    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(50001);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    ret = bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) std::cout << "BIND ERROR\n"; else std::cout << "BIND OK\n";

    ret = listen(socket_fd, 50);
    if (ret < 0) std::cout << "LISTEN ERROR\n"; else std::cout << "LISTEN OK\n";

    struct sockaddr_in client;
    socklen_t client_size = sizeof(client);
    ret = accept(socket_fd, (struct sockaddr *)&client, &client_size);
    if (ret < 0) std::cout << "ACCEPT ERROR\n"; else std::cout << "ACCEPT OK\n";
    
    ret = send(ret, "Hello!", 6, 0);
    if (ret < 0) std::cout << "SEND ERROR\n"; else  std::cout << "SEND OK\n";
    
    // while (1)
    //     ;

}
