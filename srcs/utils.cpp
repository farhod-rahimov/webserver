#include "./headers/Header.hpp"

void ft_create_new_write_event(int kq, int fd) {
    struct kevent tmp;
    EV_SET(&tmp, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, 0);
    kevent(kq, &tmp, 1, NULL, 0, NULL);
}