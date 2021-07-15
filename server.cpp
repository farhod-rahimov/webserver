#include <sys/socket.h>  // socket
#include <sys/types.h>   // connect
#include <iostream>
#include <netinet/in.h>  // struct sockaddr_in

#include <arpa/inet.h>   // inet_addr
#include <sys/select.h>  // select
#include <fcntl.h>

#include <vector>
#include <unistd.h>


int main() {
    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    int ret;
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

    std::vector<int> client_fds;

    struct sockaddr_in client;
    socklen_t client_size = sizeof(client);
    
    int fd, res, flags;
    fd_set readfds, writefds;
    int max_d = socket_fd;
    while (1) {
        FD_ZERO(&readfds);
        // FD_ZERO(&writefds);
        FD_SET(socket_fd, &readfds);

        for (size_t i = 0; i < client_fds.size(); i++) {
            fd = client_fds[i];
            FD_SET(fd, &readfds);
            FD_SET(fd, &writefds);
            if (fd > max_d)
                max_d = fd;
        }
        // res = select(max_d + 1, &readfds, &writefds, NULL, NULL);
        res = select(max_d + 1, &readfds, NULL, NULL, NULL);
        if (res < 1) {
            if (errno != EINTR) {
                std::cout << "Select returned error\n";
            }
            else {
                std::cout << "Signal came\n";
            }
            continue ;
        }
        if (res == 0) {
            continue ;
        }
        if (FD_ISSET(socket_fd, &readfds)) {
            fd = accept(socket_fd, (struct sockaddr *)&client, &client_size);
            if (fd < 0) std::cout << "ACCEPT ERROR\n"; else std::cout << "ACCEPT OK\n";
            flags = fcntl(socket_fd, F_GETFL);            
            fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
            client_fds.push_back(fd);
        }
        for (size_t i = 0; i < client_fds.size(); i++) {
            fd = client_fds[i];
            if (FD_ISSET(fd, &readfds)) {
                std::cout << "CLIENT SENT SOMETHING\n";
                client_fds.erase(client_fds.begin() + i);
                close(fd);
                continue ;
            }
            // if (FD_ISSET(fd, &writefds)) {
            //     ret = send(fd, "HELLO!\n", 7, 0);
            //     if (ret < 0) std::cout << "SEND ERROR\n"; else  std::cout << "SEND OK\n";
            //     std::cout << fd << std::endl;
            // }
        }
    }

    return (0);
}
