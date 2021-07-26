#include "./headers/Header.hpp"

int ft_execve_rm(const char *filename);
void ft_send_ok(Client & client);

void	ft_response_to_delete(Client & client, Server & server, Location & location, int fd) {
    (void)server;
    (void)location;
    (void)fd;
    if (ft_execve_rm(client.ReqGetPath().c_str()) < 0) {
        ft_send_not_found(client);
    }
    else 
        ft_send_ok(client);
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

void ft_send_ok(Client & client) {
	client.RespSetStatusCode("200");
	client.RespSetStatusTxt("OK");
	
	client.RespCreateFullRespTxt();	
}