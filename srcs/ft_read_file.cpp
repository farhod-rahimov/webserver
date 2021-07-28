#include "./headers/Header.hpp"

int ft_read_file(const char * filename,  std::string & content) {
    std::ifstream::char_type c;
    std::ifstream            ifs;
    int                      ret;

    content.clear(); errno = 0;
    ret = open(filename, O_WRONLY);
    if (ret == -1 && errno == EISDIR) {
        return (0);
    }
    close(ret);

    ifs.open(filename);
    if (!ifs.is_open()) {
        return (0);
    }

    while (!ifs.eof()) {
        ifs.get(c);
        if (!ifs.eof())
            content.push_back(c);
    }
    ifs.close();
    return (1);
}
