#include "./headers/Header.hpp"

bool ft_check_config_extension(std::string conf_file);
size_t ft_get_num_of_servers(std::string content_file);
void ft_get_servers(std::string & content_file, size_t & num_servers, std::vector<Server> & servers);
Server ft_get_common_settings(std::string & content_file, size_t start, size_t end);

void ft_get_value(std::string & content_file, size_t pos, std::string * val, std::string & field);
void ft_server_settings_error(std::string & str);
size_t ft_skip_spaces_tabs(std::string & content_file, size_t i);
size_t ft_skip_notspaces_nottabs_notnewlines(std::string & content_file, size_t i);

void ft_get_first_val_before_space(std::string first_field, std::string second_field, std::string & dst, std::string & src);
size_t ft_get_num_of_locations(std::string & content_file, size_t start, size_t end);
Location ft_get_location_settings(std::string & content_file, size_t start, size_t end);

void ft_print_result(std::vector<Server> & servers) {
    std::cout << "HERE\n" << servers.size() << "\n" << servers[0].getLocations().size() << "\n";
    for (size_t i = 0; i < servers.size(); i++) {
        std::cout << "-----------------------------------------------------------------" << std::endl;
        std::cout << "SERVER " << i + 1 << std::endl;
        std::cout << "host " << servers[i].getHost() << std::endl;
        std::cout << "port " << servers[i].getPort() << std::endl;
        std::cout << "server_name " << servers[i].getServerName() << std::endl;
        std::cout << "server_root " << servers[i].getServerRoot() << std::endl;
        std::cout << "default_error_status_code " << servers[i].getDefaultErrorStatusCode() << std::endl;
        std::cout << "default_error_page_path " << servers[i].getDefaultErrorPagePath() << std::endl;
        std::cout << "limit_body_size " << servers[i].getLimitBodySize() << std::endl;
        std::cout << "-----------------------------------------------------------------" << std::endl;

        for (size_t n = 0; n < servers[i].getLocations().size(); n++) {
            std::cout << "SERVER " << i + 1 << " LOCATION " << n + 1 << std::endl;
            std::cout << "allowed_methods " << servers[i].getLocations()[n].getAllowedMethods() << std::endl;
            std::cout << "redirection " << servers[i].getLocations()[n].getRedirection() << std::endl;
            std::cout << "redirection_status_code " << servers[i].getLocations()[n].getRedirectionStatusCode() << std::endl;
            std::cout << "path " << servers[i].getLocations()[n].getPath() << std::endl;
            std::cout << "autoindex " << servers[i].getLocations()[n].getAutoindex() << std::endl;
            std::cout << "default_file " << servers[i].getLocations()[n].getDefaultFile() << std::endl;
            std::cout << "cgi_path " << servers[i].getLocations()[n].getCgiPath() << std::endl;
            std::cout << "cgi_extension " << servers[i].getLocations()[n].getCgiExtension() << std::endl;
            std::cout << "location_root " << servers[i].getLocations()[n].getLocationRoot() << std::endl;
            std::cout << "upload_directory " << servers[i].getLocations()[n].getUploadDirectory() << std::endl;
            std::cout << "-----------------------------------------------------------------" << std::endl;
        }
    }
}

void ft_parse(std::vector<Server> & servers, const char *conf_file) {
    std::string content_file;
    size_t num_servers;

    if (!ft_read_file(conf_file, content_file)) {
        std::cerr << "Cannot open config file '" << conf_file << "'. Check its existence and access rights" << std::endl; exit(EXIT_FAILURE);
    }
    else if (!ft_check_config_extension(conf_file)) {
        std::cerr << "Config file should have '.conf' extension" << std::endl; exit(EXIT_FAILURE);
    }
    else if (!(num_servers = ft_get_num_of_servers(content_file))) {
        std::cerr << "In config file there should be at least 1 server's settings" << std::endl; exit(EXIT_FAILURE);
    }
    ft_get_servers(content_file, num_servers, servers);
    
    
    // ft_print_result(servers); //delete
    // exit(1);
}

bool ft_check_config_extension(std::string conf_file) {
    size_t pos = conf_file.find_last_of(".");
    
    if (pos == conf_file.npos || conf_file.find("conf", pos) == conf_file.npos)
        return (false);
    return (true);
}

size_t ft_get_num_of_servers(std::string content_file) {
    size_t num = 0;

    for (size_t pos = content_file.find("<server[", 0); pos != content_file.npos; num++) {
        pos = content_file.find("<server[", pos + strlen("<server[") + 1);
    }
    return (num);
}

void ft_get_servers(std::string & content_file, size_t & num_servers, std::vector<Server> & servers) {
    size_t start_server_settings = 0;
    size_t end_server_settings = 0;

    for (size_t i = 0; i < num_servers; i++) {
        start_server_settings = content_file.find("<server[", start_server_settings);
        if ((end_server_settings = content_file.find("<server[", start_server_settings + strlen("<server[") + 1)) == content_file.npos)
            end_server_settings = content_file.length();
        servers.push_back(ft_get_common_settings(content_file, start_server_settings, end_server_settings));
        start_server_settings = start_server_settings + strlen("<server[") + 1;
    }
}

Server ft_get_common_settings(std::string & content_file, size_t start, size_t end) {
    Server s;
    
    std::string * values[5] = {&s.getHost(), &s.getPort(), &s.getServerName(), &s.getDefaultErrorPagePath(), &s.getLimitBodySize()};
    std::string fields[5] = {"host: ", "port: ", "server_name: ", "default_error_page: ", "limit_body_size: "};

    size_t pos;

    for (int i = 0; i < 5; i++) {
        if ((pos = content_file.find(fields[i], start)) > end && fields[i].compare("server_name: ")) {ft_server_settings_error(fields[i]);}
        else {ft_get_value(content_file, pos + fields[i].length(), values[i], fields[i]);}
    }
    ft_get_first_val_before_space("'default error page's status code'", "'default error page path'", s.getDefaultErrorStatusCode(), s.getDefaultErrorPagePath());

    size_t num_locations;
    if (!(num_locations = ft_get_num_of_locations(content_file, start, end))) {
        std::cerr << "There should be 1 location in the server settings" << std::endl; exit(EXIT_FAILURE);
    }

    size_t location_start = content_file.find("location: ", start);
    size_t location_end;
    for (size_t i = 0; i < num_locations; i++) {
        location_start = content_file.find("location: ", location_start);
        // if (location_start < start || location_start > end)
        //     break ;
        if ((location_end = content_file.find("location: ", location_start + strlen("location: ") + 1)) > end)
            location_end = end;
        s.getLocations().push_back(ft_get_location_settings(content_file, location_start, location_end));
        location_start = location_start + strlen("location: ") + 1;
    }
    return (s);
}

Location ft_get_location_settings(std::string & content_file, size_t start, size_t end) {
    Location l;

    std::string *str_values[8] = {&l.getAllowedMethods(), &l.getRedirection(), &l.getPath(), &l.getDefaultFile(), &l.getCgiPath(), &l.getLocationRoot(), &l.getUploadDirectory(), &l.getAutoindex()};
    std::string str_fields[8] = {"allow_methods: ", "redirection: ", "location: ", "default_file: ", "cgi_path: ", "location_root: ", "upload_directory: ", "autoindex: "};
    
    size_t pos;

    for (int i = 0; i < 8; i++) {
        pos = content_file.find(str_fields[i], start);
        if (pos > end)
            continue ;
        ft_get_value(content_file, pos + str_fields[i].length(), str_values[i], str_fields[i]);
    }

    if (!l.getRedirection().length() && !l.getLocationRoot().length()) {
        std::cerr << "There should be at least one field in location. 'location root: ' or 'redirection: ''" << std::endl; exit(EXIT_FAILURE);
    }

    ft_get_first_val_before_space("'cgi extension'", "'cgi path'", l.getCgiExtension(), l.getCgiPath());
    ft_get_first_val_before_space("'redirection status code'", "'redirection address'", l.getRedirectionStatusCode(), l.getRedirection());

    // std::string print_values[10] = {l.getAllowedMethods(), l.getRedirectionStatusCode(), l.getRedirection(), l.getPath(), l.getDefaultFile(), \
    //                                 l.getCgiExtension(), l.getCgiPath(), l.getLocationRoot(), l.getUploadDirectory(), l.getAutoindex()};
    return (l);
}

void ft_get_first_val_before_space(std::string first_field, std::string second_field, std::string & dst, std::string & src) {
    size_t len = 0;
    
    if (src.length() == 0) {return ;}
    len = ft_skip_notspaces_nottabs_notnewlines(src, 0);
    if (src.find(" ", 0) == src.npos && src.length() > 0) {
        std::cerr << "There should be " << first_field << " before " << second_field << " in location" << std::endl;
        exit(EXIT_FAILURE);
    }
    dst = src.substr(0, len);
    len = ft_skip_spaces_tabs(src, len);
    src = src.substr(len);
}

size_t ft_get_num_of_locations(std::string & content_file, size_t start, size_t end) {
    size_t num = 0;

    for (size_t pos = content_file.find("location: ", start); pos < end; num++) {
        pos = content_file.find("location: ", pos + strlen("location: ") + 1);
    }
    return (num);
}

void ft_get_value(std::string & content_file, size_t pos, std::string * val, std::string & field) {
    pos = ft_skip_spaces_tabs(content_file, pos);
    size_t end_pos;
    
    end_pos = ft_skip_notspaces_nottabs_notnewlines(content_file, pos);
    if (!field.compare("default_error_page: ") || !field.compare("redirection: ") || !field.compare("cgi_path: ")) {
        end_pos = ft_skip_spaces_tabs(content_file, end_pos);
        end_pos = ft_skip_notspaces_nottabs_notnewlines(content_file, end_pos);
    }
    *val = content_file.substr(pos, end_pos - pos);
}

void ft_server_settings_error(std::string & str) {
    std::cerr << "There is no '" << str << "' field in server's settings. Each server should at least have the following fields:\n";
    std::cerr << "host, port, default_error_page, limit_body_size, location\n\n";
    std::cerr << "In location there should be at least 1 field 'location_root: ' or 'redirection: '\n";
    exit(EXIT_FAILURE);
}

size_t ft_skip_spaces_tabs(std::string & content_file, size_t i) {
    for (; content_file[i] == ' ' || content_file[i] == ' '; i++) {}
    return (i);
}

size_t ft_skip_notspaces_nottabs_notnewlines(std::string & content_file, size_t i) {
    for (; content_file[i] != ' ' && content_file[i] != ' ' && content_file[i] != '\n'; i++) {}
    return (i);
}