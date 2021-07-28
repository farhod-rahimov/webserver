#include "./headers/Header.hpp"

#include <dirent.h>

static void ft_create_content_start(std::string & content);
static void ft_push_file_name_to_content(std::string & content, std::string ref, std::string file_name);
static void ft_create_content_end(std::string & content);

int ft_show_current_dir_files(Client & client, Location & location, std::string & content) {
    std::string current_absolute_dir_name, current_relative_dir_name, tmp;
    DIR             * current_dir;
    struct dirent   * s;

    ft_create_content_start(content);

    current_absolute_dir_name = client.ReqGetPath();
    current_relative_dir_name = client.ReqGetPath();
    current_relative_dir_name.replace(0, location.getLocationRoot().length(), location.getPath());
    
    if (current_relative_dir_name.back() != '/')
        current_relative_dir_name.append("/");

    current_dir = opendir(current_absolute_dir_name.c_str());

    if (current_dir == NULL)
        return (-1);

    while ((s = readdir(current_dir)) != NULL) {
        tmp = current_relative_dir_name + s->d_name;
        ft_push_file_name_to_content(content, tmp, s->d_name);
    }
    ft_create_content_end(content);
    closedir(current_dir);
    return (0);
}

static void ft_create_content_start(std::string & content) {
    content = "<!DOCTYPE HTML>\n";
    content += "<html>\n";
    content += "<head>\n";
    content += "<meta charset=\"utf-8\">\n";
    content += "</head>\n";
    content += "<body>\n";
}

static void ft_push_file_name_to_content(std::string & content, std::string ref, std::string file_name) {
    content += "<p><a href=\""; content += ref; content += "\">";
    content += file_name; content += "</a></p>\n";
}

static void ft_create_content_end(std::string & content) {
    content += "</body>\n";
    content += "</html>\n";
}
