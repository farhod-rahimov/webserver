#include <sys/socket.h>  // socket
#include <sys/types.h>   // connect
#include <iostream>
#include <netinet/in.h>  // struct sockaddr_in

#include <arpa/inet.h>   // inet_addr
#include <unistd.h>

int main() {
    int socket_fd;
    int ret;
    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
        std::cout << "SOCKET_CLIENT ERROR\n";
    else
        std::cout << "SOCKET_CLIENT OK\n";

    struct sockaddr_in server_addr;
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(50001);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    ret = connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
        std::cout << "CONNECT_CLIENT ERROR\n";
    else
        std::cout << "CONNECT_CLIENT OK\n";

    ret = send(socket_fd, "HELLO SERVER!\n", 14, 0);
    if (ret < 0) std::cout << "SEND ERROR\n"; else  std::cout << "SEND OK\n";
    
    char buf[1000];
    ret = recv(socket_fd, &buf, 1000, 0);
    buf[ret] = '\0';
    if (ret < 0)  std::cout << "RECEIVE_CLIENT ERROR\n"; else std::cout << "RECEIVE_CLIENT OK\n";
    std::cout << buf << std::endl;


    // while (1)
    //     ;
}

// cpp -c main.cpp && cpp -o server main.o && cpp -c client.cpp && cpp -o client client.o
// cpp -c server.cpp && cpp -o server server.o && cpp -c client.cpp && cpp -o client client.o
// lsof -i | grep server
// rm *.o server client