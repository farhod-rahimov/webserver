#include "./headers/Header.hpp"

void ft_save_file(Client & client, Location & location);
void ft_work_with_cgi(Client & client, Server & server, Location & location, int fd);
std::string ft_get_req_path_extension(Client & client);
void ft_send_file_was_created(Client & client);

void	ft_response_to_post(Client & client, Server & server, Location & location, int fd) {
    std::string req_path_extension;

    req_path_extension = ft_get_req_path_extension(client);
    if (req_path_extension == location.getCgiExtension() && !location.getCgiExtension().empty()) {
        ft_work_with_cgi(client, server, location, fd);
        ft_send_ok(client);
        return ;
    }
    ft_save_file(client, location);
    ft_send_file_was_created(client);
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

void ft_work_with_cgi(Client & client, Server & server, Location & location, int fd) {
    Cgi cgi(client, server, location, fd);

    cgi.cgiInit();
}

std::string ft_get_req_path_extension(Client & client) {
    std::string ext;
    size_t i = 0;
    size_t pos = client.ReqGetPath().rfind(".");

    if (pos != std::string::npos) {
        ext = client.ReqGetPath().substr(pos);
        if (ext.back() == '/')
            ext.pop_back();
    }
    if (ext[i] == '.')
        i++;
    for (; ext[i] >= 'a' && ext[i] <= 'z'; i++) {}
    ext = ext.substr(0, i);
    std::cout << "EXT = " << ext << "\n";
    return (ext);
}

void ft_send_file_was_created(Client & client) {
	const char * content = "<html>\nYour file was created. It is here <p><a href=\"/uploaded_files\">Your file</a></p>\n</html>";
	
	client.RespSetStatusCode("201");
	client.RespSetStatusTxt("CREATED");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}
