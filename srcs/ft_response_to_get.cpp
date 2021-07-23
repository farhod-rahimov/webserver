#include "./headers/Header.hpp"

int ft_read_file(const char * filename,  std::string & content);
void ft_send_not_found(Client & client);

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

void ft_response_to_get(Client & client) {
    std::string content;
    std::string content_type = "text/";
    std::string extension;

    if (client.ReqGetPath().length() == 1) {
        client.ReqGetPath().replace(client.ReqGetPath().find("/"), 1, "./hosted_website/indexCopy.html");  // нужно перебирать все .html файлы, к-рые указаны в конфиг файле
    }
    else
        client.ReqGetPath().replace(client.ReqGetPath().find("/"), 1, "./hosted_website/"); // ./hosted_website should be taken from config file

    std::cout << client.ReqGetPath() << std::endl;

    if (!ft_read_file(client.ReqGetPath().c_str(), content))
        ft_send_not_found(client);
    else {
        size_t pos = client.ReqGetPath().find_last_of(".");
        extension =  client.ReqGetPath().substr(++pos);
        std::cout << "extension = " << extension << std::endl;
        content_type += extension;
        ft_create_header(content_type.c_str(), client, content);
    }
}

