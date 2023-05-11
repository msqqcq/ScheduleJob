/**
 * Copyright @2019 Exserver All Rights Reserved.
 */


#ifndef EXSERVER_EX_EPOLL_H
#define EXSERVER_EX_EPOLL_H

#include <estring.h>
#ifdef __linux__
#include <sys/epoll.h>

int event_init();
int event_add(int fd, int sd);
int event_enable_write(int fd, int sd);
int event_disable_write(int fd, int sd);
int event_delete(int fd, int sd);
void event_loop(int fd, void (*func)(int, int, int));

#endif
#endif /* EXSERVER_EX_EPOLL_H */
