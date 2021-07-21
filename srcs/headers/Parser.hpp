#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

class Parser {
    public:
        Parser();
        ~Parser();

        Parser(const Parser & src);
        
        Parser & operator = (const Parser & src);

        std::string & GetHost(void);
        std::string & GetPort(void);
        std::string & GetServerName(void);
        std::string & GetDefaultErrorPage(void);
        std::string & GetLimitBodySize(void);
        std::string & GetRoot(void);
        
        std::string & GetAllowMethods(void);
        std::string & GetRedirection(void);
        std::string & GetPath(void);
        std::string & GetAutoindex(void);
        std::string & GetDefaultFile(void);
        std::string & GetDefaultFile(void);
        std::string & GetCgiPath(void);

    private:
        std::string _host;
        std::string _port;
        std::string _server_name;
        std::string _default_error_page;
        std::string _limit_body_size;
        std::string _root;
};

class Location {
        std::string _allow_methods;
        std::string _redirection;
        std::string _path;
        std::string _autoindex;
        std::string _default_file;
        std::string _cgi_path;
};

#endif