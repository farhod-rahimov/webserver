#include "./headers/Header.hpp"

void ft_get_content_and_content_type(Client & client, Server & server, Location & location, std::string & content);
void ft_get_content_type(std::string & content_type, std::string & extension);
void ft_create_header(const char * content_type, Client & client, std::string & content);
void ft_send_not_found(Client & client);

int ft_show_current_dir_files(Client & client, Location & location, std::string & content);

void ft_response_to_get(Client & client, Server & server, Location & location, int fd) {
    std::string content, tmp;

    (void)fd;
    
    std::string req_path_extension;
    req_path_extension = ft_get_req_path_extension(client);
    std::cout << req_path_extension;
    if (req_path_extension == location.getCgiExtension() && !location.getCgiExtension().empty()) {
        ft_work_with_cgi(client, server, location, fd);
        ft_send_ok(client);
    	std::cout << "																				ALKLKAMDCKLMADSKLCMDALKCMAKLDMC\n";
        return ;
    }

    if (!location.getRedirection().empty()) {
        client.RespSetProtocol("HTTP/1.1");
        client.RespSetStatusCode(location.getRedirectionStatusCode());
        client.RespSetStatusTxt("MOVED");
        if (location.getRedirection().back() == '/')
            location.getRedirection().pop_back();

        std::cout << client.RespCreateFullRespTxt();
        return ;
    }
    ft_get_content_and_content_type(client, server, location, content);
}

void ft_get_content_and_content_type(Client & client, Server & server, Location & location, std::string & content) {
    std::string content_type, extension, read_file = client.ReqGetPath();
    size_t pos;
    std::string req_path = client.ReqGetPath();
    std::string location_root = location.getLocationRoot();

    if (req_path.back() != '/') {req_path.append("/");}
    if (location_root.back() != '/') {location_root.append("/");}

    std::string status_code;

    if (!ft_read_file(read_file.c_str(), content)) {
        read_file = server.getDefaultErrorPagePath();
        if (errno == EISDIR && req_path == location_root) { // is directory
            read_file = location.getDefaultFile();
            if (!ft_read_file(read_file.c_str(), content)) {
                read_file = server.getDefaultErrorPagePath();

                if (location.getAutoindex().find("on") != location.getAutoindex().npos) {
                    if (ft_show_current_dir_files(client, location, content) == -1) {
                        ft_send_not_found(client); return ;
                    }
                    else {
                        read_file.clear();
                        content_type = "text/html";
                    }
                }
                else if (!ft_read_file(read_file.c_str(), content)) {
                    ft_send_not_found(client); return ;
                }
                else {
                    status_code = server.getDefaultErrorStatusCode();
                }
            }
        }
        else if (!ft_read_file(read_file.c_str(), content)) {
            ft_send_not_found(client); return ;
        }
    }
    if ((pos =read_file.find_last_of(".")) != read_file.npos) {
        extension =  read_file.substr(++pos);
        // std::cout << "extension = " << extension << std::endl;
        ft_get_content_type(content_type, extension);
    }
    // std::cout << content << "\n"; exit(1);
    ft_create_header(content_type.c_str(), client, content);
    // std::cout << content.length() << " " << content.size() << " SJANCXKJASNCKJASNCKASJCNAJKSCNJKS\n"; exit(1);
    if (status_code.length())
        client.RespGetStatusCode() = status_code;
}

void ft_get_content_type(std::string & content_type, std::string & extension) {
    const char * application_extensions [] = { "epub", "js", "json", "jar", "doc", "pdf", "xml", "docx", \
                                               "rar", "zip", "tar", NULL};
    
    const char * audio_extensions [] = { "mp3", "mp4a", "mpga", "wma", "wax", "wav", NULL};
    
    const char * image_extensions [] = { "bmp", "jpg", "jpeg", "gif", "png", "xpm", "ico", NULL};
    
    const char * text_extensions [] = { "html", "css", "csv", "txt", "rtx", "yaml", "bmp", NULL};
    
    // const char * video_extensions [] = { "3gp", "mp4", "mpeg", "avi", "movie", NULL};

    size_t i;

    content_type = "application/";
    for (i = 0; application_extensions[i] != NULL && \
        extension.compare(application_extensions[i]) != 0; i++) {}
    
    if (application_extensions[i] == NULL) {
        content_type = "audio/";
        for (i = 0; audio_extensions[i] != NULL && \
            extension.compare(audio_extensions[i]) != 0; i++) {}
        
        if (audio_extensions[i] == NULL) {
            content_type = "image/";
            for (i = 0; image_extensions[i] != NULL && \
                extension.compare(image_extensions[i]) != 0; i++) {}
            
            if (image_extensions[i] == NULL) {
                content_type = "text/";
                for (i = 0; text_extensions[i] != NULL && \
                    extension.compare(text_extensions[i]) != 0; i++) {}

                if (text_extensions[i] == NULL) {
                    // content_type = "video/";
                    // for (i = 0; video_extensions[i] != NULL && \
                    //     extension.compare(video_extensions[i]) != 0; i++) {}
                    
                    // if (video_extensions[i] == NULL) {
                        content_type = "application/octet-stream";
                        return ;
                    // }
                }
            }
        }
    }
    content_type += extension;
}

void ft_create_header(const char * content_type, Client & client, std::string & content) {

    client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("200");
	client.RespSetStatusTxt("OK");
	client.RespSetContentType(content_type);
	client.RespSetContentLength(content.length());
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();
}

void ft_send_not_found(Client & client) {
	const char * content = "<html>\nError 404 Not Found.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetStatusCode("404");
	client.RespSetStatusTxt("NOT FOUND");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}

void ft_send_too_long_body(Client & client, int fd, int kq) {
	const char * content = "<html>\nError 413 Request Entity Too Large.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetStatusCode("413");
	client.RespSetStatusTxt("Request Entity Too Large");
	client.RespSetContentType("text/html");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();

    send(fd, client.RespGetFullRespTxt().c_str(), client.RespGetRemainedToSent(), 0);

    struct kevent tmp;
    EV_SET(&tmp, fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);
    kevent(kq, &tmp, 1, NULL, 0, NULL);
}