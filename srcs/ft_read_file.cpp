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
    close(ret);

    ifs.open(filename); // should be the full paath of the file
    if (!ifs.is_open()) {
        return (0);
    }
    ofs.open(tmp);

    std::ifstream::char_type c;

    while (!ifs.eof()) {
        ifs.get(c);
        if (!ifs.eof())
            content.push_back(c);
    }
    std::cout << "                                                              LEN = " << content.length() << std::endl;
    ifs.close();
    ofs.close();
    return (1);
}