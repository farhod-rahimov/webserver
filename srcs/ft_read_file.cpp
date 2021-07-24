#include "./headers/Header.hpp"

int ft_read_file(const char * filename,  std::string & content) {

    std::ifstream ifs;
    std::ofstream ofs;
    std::string tmp;
    int ret;

    content.clear();
    errno = 0;
    ret = open(filename, O_WRONLY);
    if (ret == -1 && errno == EISDIR) {
        return (0);
    }

    ifs.open(filename); // should be the full paath of the file
    if (!ifs.is_open()) {
        return (0);
    }
    ofs.open(tmp);

    while (!ifs.eof()) {
        std::getline(ifs, tmp);
        tmp.append("\n");
        content += tmp;
        if (ifs.eof())
            break ;
        // std::cout << tmp;
    }
    ifs.close();
    ofs.close();
    return (1);
}