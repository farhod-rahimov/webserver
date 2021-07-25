#include "./headers/Header.hpp"

bool ft_check_chunk_end(std::string & buf, size_t first_dclrf);
bool ft_check_body_end(std::string & buf, size_t first_dclrf);
size_t ft_count_body_length(std::string & buf, size_t first_dclrf);

#define CLRF "\r\n"
#define DOUBLE_CLRF "\r\n\r\n"

bool	ft_check_end_request(std::string & buf) {
    size_t first_dclrf;

    first_dclrf = buf.find(DOUBLE_CLRF);
    if (first_dclrf == buf.npos)
        return (false);

    if (buf.find("Transfer-Encoding: chunked") != buf.npos)
        return (ft_check_chunk_end(buf, first_dclrf));
    if (buf.find("Content-Length: ") != buf.npos)
        return (ft_check_body_end(buf, first_dclrf + 4));
    return (true);
}

bool ft_check_chunk_end(std::string & buf, size_t first_dclrf) {
    size_t last_dclrf;

    last_dclrf = buf.rfind(DOUBLE_CLRF);
    if (last_dclrf <= first_dclrf || last_dclrf == buf.npos)
        return (0);
    if (buf[last_dclrf - 1] == '0' && buf[last_dclrf - 2] == '\n' && buf[last_dclrf - 3] == '\r')
        return (true);
    return (false);
}

bool ft_check_body_end(std::string & buf, size_t first_dclrf) {
    size_t len;
    size_t con_len;
    std::string txt_len;

    con_len = buf.find("Content-Length: ") + strlen("Content-Length: ");
    
    for (len = 0; buf[con_len] != '\r' && buf[con_len + 1] != '\n' && con_len < buf.size(); con_len++) {
        len++;
    }
    txt_len = buf.substr(con_len - len, len);
    con_len = std::atoi(txt_len.c_str());
    std::cout << "CON_LEN " << con_len << std::endl;
    // exit(1);
    if (con_len == ft_count_body_length(buf, first_dclrf))
        return (true);
    return (false);
}

size_t ft_count_body_length(std::string & buf, size_t first_dclrf) {
    size_t len = buf.size() - first_dclrf;
    
    std::cout << "LEN " << len << std::endl;
    // exit(1);
    return (len);
}