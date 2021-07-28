#include "./headers/Header.hpp"

static void ft_save_file(Client & client, Location & location);

void	ft_response_to_post(Client & client, Server & server, Location & location, int fd) {
    std::string req_path_extension;

    req_path_extension = ft_get_req_path_extension(client);
    if (req_path_extension == location.getCgiExtension() && !location.getCgiExtension().empty()) {
        if (ft_work_with_cgi(client, server, location, fd) < 0)
            ft_send_internal_error(client);
        else
            ft_send_204_not_content(client);
        return ;
    }
    ft_save_file(client, location);
    ft_send_file_was_created(client);
}

static void ft_save_file(Client & client, Location & location) {
    std::string     file_name;
    std::ofstream   ofs;

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
