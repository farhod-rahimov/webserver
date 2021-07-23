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

        bool                    & getCheckFlag(void);
        std::string             & getHost(void); // mandatory
        std::string             & getPort(void); // mandatory
        std::string             & getServerName(void);
    std::string             & getServerRoot(void);              // лишний ?
        std::string             & getDefaultErrorStatusCode(void); // mandatory
        std::string             & getDefaultErrorPagePath(void); // mandatory
        std::string             & getLimitBodySize(void); // mandatory  // in bytes
        
        std::vector<location>   & getLocations(void);
        std::map<int, Client>   & getClients(void);

    private:
        bool                    _check_flag;
        std::string             _host;
        std::string             _port;
        std::string             _server_name;
        std::string             _server_root;                       // лишний ?
        std::string             _default_error_status_code;
        std::string             _default_error_page_path;
        std::string             _limit_body_size;

        std::vector<location>   _locations;
        std::map<int, Client>   _clients;
};

struct Location {
    public:
        Location();
        Location(const Location * src);
        
        Location & operator = (const Location & src);

        std::string & getAllowedMethods(void);
        std::string & getRedirection(void);
        std::string & getRedirectionStatusCode(void); //
        std::string & getPath(void);
        std::string & getAutoindex(void);
        std::string & getDefaultFile(void);
        std::string & getCgiPath(void);
        std::string & getCgiExtension(void); //
        std::string & getLocationRoot(void);
        std::string & getUploadDirectory(void); // 10

        std::string _allowed_methods;
        std::string _redirection;
        std::string _redirection_status_code;
        std::string _path;
        std::string _autoindex;
        std::string _default_file;
        std::string _cgi_path;
        std::string _cgi_extension;
        std::string _location_root;
        std::string _upload_directory;
};

#endif