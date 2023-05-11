/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#ifndef EX_SOCKET_H
#define EX_SOCKET_H

#include <ck_http.h>

#define SOCKET_FD_LEN CMSG_LEN(sizeof(int))

/**
 * @brief Introduction
 * Some Macros and function for socket's create, listen, connect or accept operation
 */
#define socket_accept(fd, addr, len) accept(fd, addr, len)
int socket_nonblock(int fd);
int socket_create(const char *host, uint16_t port);
int socket_listen(int fd, int bl);
int socket_connect(int fd, const char *host, uint16_t port);

/**
 * @brief Introduction
 * Below are some APIs for Send fd between two file descriptor
 */
int socket_send_fd(int fd, int fd_to_send);
int socket_recv_fd(int fd);

/**
 * @brief Introduction
 * Below are some APIs for socket read and write
 */
char *socket_read(int  fd, long *len);
long  socket_write(int fd, char *res, unsigned long res_len);

#endif /* EX_SOCKET_H */

#pragma clang diagnostic pop