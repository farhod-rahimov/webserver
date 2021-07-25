#include "./headers/Header.hpp"

void ft_execve_add_post(Client & client);

void ft_response_to_post(Client & client) {
    std::string content;
    std::string content_type = "text/";
    std::string extension;
    ft_execve_add_post(client);

    if (!ft_read_file("./hosted_website/posts.txt", content))
        ft_send_not_found(client);
    else {
        size_t pos = client.ReqGetPath().find_last_of(".");
        extension =  client.ReqGetPath().substr(++pos);
        std::cout << "extension = " << extension << std::endl;
        content_type += extension;
        ft_create_header(content_type.c_str(), client, content);
    }
}

void ft_execve_add_post(Client & client) {
    int     status;
    pid_t   child;

    client.ReqGetPath().replace(client.ReqGetPath().find("/"), 1, "./hosted_website/");

    char **argv = new char * [3];
    argv[0] = new char [client.ReqGetPath().length()];
    argv[1] = new char [client.ReqGetContent().length()];
    argv[2] = NULL;

    strcpy(argv[0], client.ReqGetPath().c_str());
    strcpy(argv[1], client.ReqGetContent().c_str());

    std::cout << argv[0] << "\n";
    std::cout << argv[1] << "\n";
    if ((child = fork()) == 0) {
        if (execve(argv[0], argv, NULL) == -1) {
            std::cerr << "Cannot exceve " << argv[0] << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if (child == -1) {
        std::cerr << "Fork Error\n";
    }
    else {
        waitpid(child, &status, 0);
    }
    delete [] argv[0];
    delete [] argv[1];
    delete [] argv;
}
