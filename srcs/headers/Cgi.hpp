#ifndef CGI_HPP
#define CGI_HPP

#include "Header.hpp"

class Cgi {
    public:
        Cgi(Client & client, Server & server, Location & location, int fd);
        ~Cgi();

        Cgi(const Cgi & src);

        Cgi & operator = (const Cgi & src);

        int  cgiInit();
        void cgiCreateEnv(void);
        void cgiConvertEnv(void);
        int  cgiExecute(void);
        void cgiClearEnv(void);

    private:
        Client      _resp_client;
        Server      _resp_server;
        Location    _resp_location;

        char **     _env;
        int         _fd;
        std::map<std::string, std::string> _tmp_env;
};

#endif