#include "./headers/Header.hpp"

void ft_get_content_and_content_type(Client & client, Server & server, Location & location, std::string & content);
void ft_get_content_type(std::string & content_type, std::string & extension);
void ft_create_header(const char * content_type, Client & client, std::string & content);
void ft_send_not_found(Client & client);

void ft_response_to_get(Client & client, Server & server, Location & location) {
    std::string content;

    ft_get_content_and_content_type(client, server, location, content);
    std::cout << "CONTENT TYPE " << client.RespGetContentType() << std::endl;
}

void ft_get_content_and_content_type(Client & client, Server & server, Location & location, std::string & content) {
    std::string content_type, extension, read_file = client.ReqGetPath();
    size_t pos;

    if (!ft_read_file(read_file.c_str(), content)) {
        read_file = server.getDefaultErrorPagePath();
        if (errno == EISDIR) { // is directory
            read_file = location.getDefaultFile();
            if (!ft_read_file(read_file.c_str(), content)) {
                read_file = server.getDefaultErrorPagePath();
                if (!ft_read_file(read_file.c_str(), content)) {
                    ft_send_not_found(client); ft_create_header("text/html", client, content); return ;
                }
            }
        }
        else if (!ft_read_file(read_file.c_str(), content)) {
            ft_send_not_found(client); ft_create_header("text/html", client, content); return ;
        }
    }
    if ((pos =read_file.find_last_of(".")) != read_file.npos) {
        extension =  read_file.substr(++pos);
        std::cout << "extension = " << extension << std::endl;
        ft_get_content_type(content_type, extension);
    }
    ft_create_header(content_type.c_str(), client, content);
}

void ft_get_content_type(std::string & content_type, std::string & extension) {
    const char * application_extensions [] = { "epub", "js", "json", "jar", "doc", "pdf", "xml", "docx", \
                                               "rar", "zip", "tar", NULL};
    
    const char * audio_extensions [] = { "mp3", "mp4a", "mpga", "wma", "wax", "wav", NULL};
    
    const char * image_extensions [] = { "bmp", "jpg", "jpeg", "gif", "png", "xpm", "ico", NULL};
    
    const char * text_extensions [] = { "html", "css", "csv", "txt", "rtx", "yaml", "bmp", NULL};
    
    const char * video_extensions [] = { "3gp", "mp4", "mpeg", "avi", "movie", NULL};

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
                    content_type = "video/";
                    for (i = 0; video_extensions[i] != NULL && \
                        extension.compare(video_extensions[i]) != 0; i++) {}
                    
                    if (video_extensions[i] == NULL) {
                        content_type = "application/octet-stream";
                        return ;
                    }
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
	client.RespSetConnection("Connection: keep-alive");
	client.RespSetContentType(content_type);
	client.RespSetContentLength(content.length());
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();
}

void ft_send_not_found(Client & client) {
	const char * content = "<html>\nError 404 Not Found.\nThe request cannot be carried out by the web server\n</html>";
	
	client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("404");
	client.RespSetStatusTxt("NOT FOUND");
	client.RespSetConnection("Connection: keep-alive");
	client.RespSetContentType("text/html; charset=utf-8");
	client.RespSetContentLength(strlen(content));
	client.RespSetContent(content);
	
	client.RespCreateFullRespTxt();	
}