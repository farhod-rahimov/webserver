#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>
#include <map>

#include "./Client.hpp"

typedef struct Location location;

class Server {
    public:
        Server();
        virtual ~Server();

        Server(const Server & src);
        
        Server & operator = (const Server & src);

        std::string             & GetHost(void);
        std::string             & GetPort(void);
        std::string             & GetServerName(void);
        std::string             & GetServerRoot(void);
        std::string             & GetDefaultErrorPage(void);
        std::string             & GetLimitBodySize(void);
        
        std::vector<location>   & GetLocations(void);
        std::map<int, Client>     & GetClients(void);

    private:
        std::string             _host;
        std::string             _port;
        std::string             _server_name;
        std::string             _server_root;
        std::string             _default_error_page;
        std::string             _limit_body_size;

        std::vector<location>   _locations;
        std::map<int, Client>   _clients;
};

struct Location {
    public:
        Location();

        std::string & GetAllowMethods(void);
        std::string & GetRedirection(void);
        std::string & GetPath(void);
        bool        & GetAutoindex(void);
        std::string & GetDefaultFile(void);
        std::string & GetCgiPath(void);
        std::string & GetLocationRoot(void);
        bool        & GetUploadAccept(void);
        std::string & GetUploadDirectory(void);

        std::string _allow_methods;
        std::string _redirection;
        std::string _path;
        bool        _autoindex;
        std::string _default_file;
        std::string _cgi_path;
        std::string _location_root;
        bool        _upload_accept;
        std::string _upload_directory;
};

#endif