#include <iostream>

#include "./srcs/headers/Header.hpp"

void ft_create_header_resp_cgi(std::string & content) {
    std::cout << "HTTP/1.1 200 OK\r\n";
    std::cout << "Content-Length: " << content.length() << "\r\n";
    std::cout << "Content-Type: text/plain\r\n";
    std::cout << "\r\n";
    std::cout << content;
    std::cout << "\r\n";
}

int main(int argc, char **argv, char **e) {
    (void)argc;
    (void)argv;
    std::string content;

    for (size_t i = 0; e[i] != NULL; i++) {
        content += e[i];
        content += "\n";
    }
    ft_create_header_resp_cgi(content);
    return (0);
}