#include <fstream>
#include <iostream>

void ft_replace_plus_to_space(std::string & str) {
    for (std::string::iterator it = str.begin(); it != str.end(); it++) {
        if (*it == '+')
            *it = ' ';
    }
}

int main(int argc, char **argv) {
    std::string raw = argv[1];

    if (argc == 1)
        return (0);
    ft_replace_plus_to_space(raw);
    std::string date = raw.substr(0, raw.find("&"));
    std::string text = raw.substr(date.length() + 1);

    std::ofstream ofs;

    ofs.open("/Users/btammara/webserver/hosted_website/posts.txt", std::ios_base::app);
    if (!ofs.is_open()) {
        std::cout << "Cannot open for append posts.txt file\n";
        return (EXIT_FAILURE);
    }
    ofs.write(date.c_str(), date.length());
    ofs.write("\n", 1);
    ofs.write(text.c_str(), text.length());
    ofs.write("\n---------------------------------------------------------------------\n", 71);
    ofs.close();
    return (EXIT_SUCCESS);
}