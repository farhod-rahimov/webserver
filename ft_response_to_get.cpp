#include "Header.hpp"

void ft_response_to_get(Client & client) {
    std::ifstream ifs;
    std::ofstream ofs;
    std::string tmp;
    std::string full;

    ifs.open("./root/index.html"); // should be the full paath of the file
    if (!ifs.is_open()) {
        std::cerr << "FILE DOESNT EXIT \n"; // 404 page
        return ;
    }
    ofs.open(tmp);

    while (!ifs.eof()) {
        std::getline(ifs, tmp);
        tmp.append("\n");
        full += tmp;
        if (ifs.eof())
            break ;
        std::cout << tmp;
    }
    ifs.close();
    ofs.close();

    client.RespSetProtocol("HTTP/1.1");
	client.RespSetStatusCode("200");
	client.RespSetStatusTxt("OK");
	client.RespSetConnection("Connection: keep-alive");
	client.RespSetContentType("text/html; charset=utf-8");
	client.RespSetContentLength(strlen(full.c_str()));
	client.RespSetContent(full);
	
	client.RespCreateFullRespTxt();

    std::cout << "TMP_SIZE " << full.size() << "\n";
    std::cout << "Content len " << client.RespGetContentLength() << "\n";
    std::cout << "Full len " << client.RespGetRemainedToSent() << "\n";
}
