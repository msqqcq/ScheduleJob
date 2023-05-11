/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */


#include <ck_http.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

unsigned long http_response(char **res, int code, char *msg,
    unsigned long msg_len, char *body, unsigned long body_len, int keep_alive, int n, ...)
{
    long res_len;
    char    buf[BUFFER_SIZE],  *header;
    va_list arg;
    
    e_memzero(buf, sizeof(buf));
    sprintf(buf, "%d ", code);
/**
 * @brief Introduction
 * HTTP1.1 200 OK\r\n
 */
    e_concat_str(res, E_STRL("HTTP/1.1 "), 1);
    e_concat_str(res, buf, 4,              0);
    e_concat_str(res, msg, msg_len,        0);
    res_len = e_concat_str(res, E_STRL("\r\n"),      0);
    
/**
 * @brief Introduction
 * HTTP headers
 */
    va_start(arg, n);
    while(n--)
    {
        header = va_arg(arg, char *);
        e_concat_str(res, header, strlen(header),    0);
        res_len = e_concat_str(res, E_STRL("\r\n"),  0);
    }
    va_end(arg);
/**
 * @brief Introduction
 * Add Connection header
 */
    if (keep_alive)
    {
        e_concat_str(res, E_STRL("Connection: keep-alive\r\n"), 0);
    }
    else
    {
        e_concat_str(res, E_STRL("Connection: close\r\n"),     0);
    }
    
/**
 * @brief Introduction
 * Add Content-Length header
 */
    if ( body_len )
    {
        e_memzero(buf, sizeof(buf));
        sprintf(buf, "Content-Length:%ld\r\n\r\n", body_len);
        e_concat_str(res, buf, strlen(buf), 0);
        res_len = e_concat_str(res, body, body_len, 0);
    }
    
    return ( unsigned long )res_len;
}


unsigned long http_cknit_response(char **res, int code, char *msg,
    unsigned long msg_len, char *body, unsigned long body_len, int keep_alive)
{
    return http_response(res, code, msg, msg_len, body, body_len, keep_alive,
        2,
        "Server: cknit/1.0",
        "Content-Type: application/json;charset=UTF-8"
    );
}

#pragma clang diagnostic pop