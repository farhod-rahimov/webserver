#include "./headers/Header.hpp"

void ft_print_result(std::vector<Server> & servers) {
    std::cout << "HERE\n" << servers.size() << "\n" << servers[0].getLocations().size() << "\n";
    for (size_t i = 0; i < servers.size(); i++) {
        std::cout << "-----------------------------------------------------------------" << std::endl;
        std::cout << "SERVER " << i + 1 << std::endl;
        std::cout << "host " << servers[i].getHost() << std::endl;
        std::cout << "port " << servers[i].getPort() << std::endl;
        std::cout << "server_name " << servers[i].getServerName() << std::endl;
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