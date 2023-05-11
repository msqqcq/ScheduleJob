/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef EXSERVER_EX_KQUEUE_H
#define EXSERVER_EX_KQUEUE_H

#include <config.h>

#ifndef __linux__

#include <sys/time.h>
#include <sys/event.h>

int  event_init();
int  event_add(int fd, int sd);
int  event_enable_write(int fd, int sd);
int  event_disable_write(int fd, int sd);
int  event_delete(int fd, int sd);
void event_loop(int fd, void (*func)(int, int, int));

#endif

#endif /* EXSERVER_EX_KQUEUE_H */
