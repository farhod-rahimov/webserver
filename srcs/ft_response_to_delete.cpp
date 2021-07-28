#include "./headers/Header.hpp"

static int ft_execve_rm(const char *filename);

void	ft_response_to_delete(Client & client) {
    int ret;

    errno = 0;
    ret = open(client.ReqGetPath().c_str(), O_RDWR);
    if (ret < 0 && errno == ENOENT)           // ENOENT -> no such file or directory
        ft_send_not_found(client);
    else if (ret < 0 && errno == EACCES)      // EACCES -> permission denied
        ft_send_forbidden(client);
    else if (ft_execve_rm(client.ReqGetPath().c_str()) != 0)
        ft_send_internal_error(client);
    else 
        ft_send_204_not_content(client);
}

static int ft_execve_rm(const char *filename) {
    int     status;
    pid_t   child;

    char **argv = new char * [3];
    argv[0] = new char [strlen("/bin/rm") + 1];
    argv[1] = new char [strlen(filename) + 1];
    argv[2] = NULL;

    strcpy(argv[0], "/bin/rm");
    strcpy(argv[1], filename);

    if ((child = fork()) == 0) {
        if (execve(argv[0], argv, NULL) == -1) {
            std::cerr << "Error. Cannot execve " << argv[0] << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    else if (child == -1) {
        std::cerr << "Error. Cannot call fork() \n";
        return (EXIT_FAILURE);
    }
    else {
        waitpid(child, &status, 0);
    }
    delete [] argv[0]; delete [] argv[1]; delete [] argv;
    return (WEXITSTATUS(status));
}
