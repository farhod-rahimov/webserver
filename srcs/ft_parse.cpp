#include "./headers/Header.hpp"

bool ft_check_config_extension(std::string conf_file);
size_t ft_get_num_of_servers(std::string content_file);
void ft_get_servers(std::string & content_file, size_t & num_servers, std::vector<Server> & servers);
Server ft_get_common_settings(std::string & content_file, size_t start, size_t end);

void ft_get_value(std::string & content_file, size_t pos, std::string * val, std::string & field);
void ft_server_settings_error(std::string & str);
size_t ft_skip_spaces_tabs(std::string & content_file, size_t i);
size_t ft_skip_notspaces_nottabs_notnewlines(std::string & content_file, size_t i);

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
    size_t start_sever_setting = 0;
    size_t end_sever_setting = 0;

    for (size_t i = 0; i < num_servers; i++) {
        start_sever_setting = content_file.find("<server[", start_sever_setting);
        end_sever_setting = content_file.find("<server[", start_sever_setting + strlen("<server[") + 1);

        servers.push_back(ft_get_common_settings(content_file, start_sever_setting, end_sever_setting));
    }
}

size_t ft_get_num_of_locations(std::string & content_file, size_t start, size_t end);
Location ft_get_location_settings(std::string & content_file, size_t start, size_t end);

Server ft_get_common_settings(std::string & content_file, size_t start, size_t end) {
    Server s;
    
    std::string * values[4] = {&s.getHost(), &s.getPort(), &s.getDefaultErrorPagePath(), &s.getLimitBodySize()};
    std::string mandatory_fields[4] = {"host: ", "port: ", "default_error_page: ", "limit_body_size: "};

    size_t pos;

    for (int i = 0; i < 4; i++) {
        if ((pos = content_file.find(mandatory_fields[i], start)) > end) {ft_server_settings_error(mandatory_fields[i]);}
        else {ft_get_value(content_file, pos + mandatory_fields[i].length(), values[i], mandatory_fields[i]);}
    }

    size_t num_locations;
    if (!(num_locations = ft_get_num_of_locations(content_file, start, end))) {
        std::cerr << "There should be 1 location in the server settings" << std::endl; exit(EXIT_FAILURE);
    }

    size_t location_start = content_file.find("location: ", start);
    size_t location_end;
    for (size_t i = 0; i < num_locations; i++) {
        location_start = content_file.find("location: ", location_start);
        if (location_start < start || location_start > end)
            break ;
        location_end = content_file.find("location: ", location_start + strlen("location: ") + 1);
        if (location_end > end)
            location_end = end;
        s.getLocations().push_back(ft_get_location_settings(content_file, location_start, location_end));
    }

    // for (int i = 0; i < 4; i++) {
    //     std::cout << values[i] << std::endl;
    // }
    // std::cout << "------------------------------------------\n";
    // exit(1);
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

    std::string print_values[10] = {l.getAllowedMethods(), l.getRedirectionStatusCode(), l.getRedirection(), l.getPath(), l.getDefaultFile(), \
                                    l.getCgiExtension(), l.getCgiPath(), l.getLocationRoot(), l.getUploadDirectory(), l.getAutoindex()};
    for (int i = 0; i < 10; i++) {
        std::cout << print_values[i] << std::endl;
    }
    exit(1);
}

size_t ft_get_num_of_locations(std::string & content_file, size_t start, size_t end) {
    size_t num = 0;

    for (size_t pos = content_file.find("location: ", start); pos != content_file.npos; num++) {
        pos = content_file.find("location: ", pos + strlen("location: ") + 1);
        if (pos > end)
            return (num);
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
    std::cerr << "host, port, default_error_page, limit_body_size, location.\n";
    std::cerr << "In location there should be at least 1 field location_root or redirection\n";
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