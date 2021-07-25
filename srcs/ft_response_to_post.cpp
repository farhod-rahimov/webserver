#include "./headers/Header.hpp"

void ft_save_file(Client & client, Location & location);

void	ft_response_to_post(Client & client, Server & server, Location & location) {
    (void)client;
    (void)server;
    (void)location;

    if ((client.ReqGetContent().find("?") != client.ReqGetContent().npos && client.ReqGetContentFileName().empty()) || \
        (client.ReqGetPath().find("?") != client.ReqGetPath().npos)) {
            if (location.getCgiPath().empty()) {
                // cgi path doesnt exist return correct error page (not implemented??)
                
                return ;
            }
            // ft_work_with_cgi();
    }
    ft_save_file(client, location);
}

void ft_save_file(Client & client, Location & location) {
    std::string file_name;
    std::ofstream ofs;

    if (!location.getUploadDirectory().empty()) {
        file_name = location.getUploadDirectory();
        if (file_name.back() != '/')
            file_name.append("/");
    }
    file_name += client.ReqGetContentFileName();

    ofs.open(file_name.c_str(), std::ios_base::trunc);
    if (ofs.is_open()) {
        ofs << client.ReqGetContent();
        ofs.close();
    }
}