#include "./headers/Header.hpp"

static bool ft_check_chunk_end(std::string & buf, size_t first_dclrf);
static bool ft_check_body_end(std::string & buf, size_t first_dclrf);
static size_t ft_count_body_length(std::string & buf, size_t first_dclrf);
static void ft_remove_extra_lines(std::string & buf, size_t first_dclrf);

bool	ft_check_end_request(std::string & buf) {
    size_t first_dclrf;

    first_dclrf = buf.find(DOUBLE_CLRF);
    if (first_dclrf == buf.npos)
        return (false);

    if (buf.find("Transfer-Encoding: chunked") != buf.npos) {
        return (ft_check_chunk_end(buf, first_dclrf));
    }
    else if (buf.find("Content-Length: ") != buf.npos)
        return (ft_check_body_end(buf, first_dclrf + 4));
    return (true);
}

bool ft_check_chunk_end(std::string & buf, size_t first_dclrf) {
    size_t last_dclrf;

    last_dclrf = buf.rfind(DOUBLE_CLRF);
    if (last_dclrf <= first_dclrf || last_dclrf == buf.npos)
        return (0);
    if (buf[last_dclrf - 1] == '0' && buf[last_dclrf - 2] == '\n' && buf[last_dclrf - 3] == '\r') {
        ft_remove_extra_lines(buf, first_dclrf);
        return (true);
    }
    return (false);
}

static bool ft_check_body_end(std::string & buf, size_t first_dclrf) {
    size_t      len;
    size_t      con_len;
    std::string txt_len;

    con_len = buf.find("Content-Length: ") + strlen("Content-Length: ");
    
    for (len = 0; buf[con_len] != '\r' && buf[con_len + 1] != '\n' && con_len < buf.size(); con_len++) {
        len++;
    }
    txt_len = buf.substr(con_len - len, len);
    con_len = std::atoi(txt_len.c_str());
    std::cout << "TOTAL		BYTES TO RECEIVE    " << con_len << std::endl;
    if (con_len == ft_count_body_length(buf, first_dclrf))
        return (true);
    return (false);
}

static size_t ft_count_body_length(std::string & buf, size_t first_dclrf) {
    size_t len = buf.size() - first_dclrf;
    
    std::cout << "REMAINED	BYTES TO RECEIVE    " << len << std::endl;
    return (len);
}

static void ft_remove_extra_lines(std::string & buf, size_t first_dclrf) {
    size_t      start;
    std::string cleared_buf;

    start = first_dclrf + 4;
    cleared_buf = buf.substr(0, start);
    size_t tmp;
    for (; start < buf.length(); start++) {
        for (; start < buf.length() && buf[start] != '\r' && buf[start + 1] != '\n'; start++) {}
        start += 2;
        tmp = buf.find(CLRF, start);
        if (start < buf.length() && tmp != std::string::npos)
            cleared_buf += buf.substr(start, tmp + 2 - start);
        start += tmp + 2 - start;
    }
    buf = cleared_buf;
}
