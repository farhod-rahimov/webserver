#include "./headers/Header.hpp"

int ft_execve_rm(const char *filename);

void ft_response_to_delete(Client & client) {
    std::string content;
    std::string extension;

    if (client.ReqGetPath().length() == 1) {
        client.ReqGetPath().replace(client.ReqGetPath().find("/"), 1, "./hosted_website/indexCopy.html");  // нужно перебирать все .html файлы, к-рые указаны в конфиг файле
    }
    else
        client.ReqGetPath().replace(client.ReqGetPath().find("/"), 1, "./hosted_website/");


    if (!ft_read_file(client.ReqGetPath().c_str(), content)) {
        content = "<html>\nFILE HAS NOT BEEN DELETED, IT DOESNT EXIST!\n</html>";
    }
    else {
        if (!ft_execve_rm(client.ReqGetPath().c_str()))
            content = "<html>\nFILE HAS BEEN DELETED!\n</html>";
        else
            content = "<html>\nFILE HAS NOT BEEN DELETED, IT DOESNT EXIST!\n</html>";
    }
    ft_create_header("text/html", client, content);
}

int ft_execve_rm(const char *filename) {
    int     status;
    pid_t   child;

    char **argv = new char * [3];
    argv[0] = new char [3];
    argv[1] = new char [strlen(filename)];
    argv[2] = NULL;

    strcpy(argv[0], "/bin/rm");
    strcpy(argv[1], filename);

    std::cout << argv[0] << "\n";
    std::cout << argv[1] << "\n";
    if ((child = fork()) == 0) {
        if (execve(argv[0], argv, NULL) == -1) {
            std::cerr << "Cannot exceve " << argv[0] << std::endl;
            exit(2);
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
    std::cout << WEXITSTATUS(status) << "SEE IT\n";
    return (WEXITSTATUS(status));
}
