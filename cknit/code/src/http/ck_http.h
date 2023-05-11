/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */


#ifndef CKNIT_CK_HTTP_H
#define CKNIT_CK_HTTP_H

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <assert.h>
#include <errno.h>

#include <stdarg.h>
#include <estring.h>

unsigned long http_response(char **res, int code, char *msg, unsigned long msg_len, char *body, unsigned long body_len, int keep_alive, int n, ...);

unsigned long http_cknit_response(char **res, int code, char *msg, unsigned long msg_len, char *body, unsigned long body_len, int keep_alive);


#endif /* CKNIT_CK_HTTP_H */
