/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 *
 * @brief Introduction
 * http_request.c or .h used for HTTP Request plus the http server
 */

#include <list.h>
#include <estring.h>
#include <errno.h>
#include <event.h>
#include <http_socket.h>
#include <http_request.h>

#if DEBUG_MODE
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#endif

inline
HTTP_HEADER *INIT_HTTP_HEADER()
{
/**
 * @brief Introduction
 * Malloc memory for HTTP HEADER
 */
    HTTP_HEADER *ptr = malloc( sizeof(HTTP_HEADER) );
    if ( !ptr ) return NULL;
    memzero(ptr, sizeof(HTTP_HEADER));
    return ptr;
}

inline
HTTP_REQUEST *INIT_HTTP_REQUEST()
{
    HTTP_REQUEST *ptr = malloc( sizeof(HTTP_REQUEST) );
    if ( !ptr ) return NULL;
    memzero(ptr, sizeof(HTTP_REQUEST));
    return ptr;
}

inline
void trash_http_header(void *data)
{
    if (!data) return ;
    HTTP_HEADER *header = (HTTP_HEADER *)data;
    e_memfree(HTTP_HEADER_KEY_P(header));
    e_memfree(HTTP_HEADER_VAL_P(header));
    e_memfree(header);
}

inline
void trash_http_request(HTTP_REQUEST *req)
{
/**
 * @brief Introduction
 * Delete the HTTP_REQUEST structure
 */
    if (!req) return ;
    e_memfree(req->body);
    e_memfree(req->version);
    e_memfree(req->uri);
    list_destroy(req->headers);
    e_memfree(req);
}

HTTP_REQUEST *
http_stream_parse(char *src, unsigned long src_len, unsigned long *next_pos)
{
    if ( !src )   return NULL;
    
    QUEUE_LIST          *headers;
    HTTP_REQUEST        *res;
    unsigned long        len,  prev_pos, colon_pos;
    unsigned char        num;
    HTTP_HEADER         *header;
    
    num        = 0;
    headers    = INIT_LIST();
    res        = INIT_HTTP_REQUEST();
    
    res->headers = headers;
    
/**
 * @brief Introduction
 * Parse the HTTP request Header LINE
 * such as the following line:
 * 
 * GET /index.html HTTP/1.1\r\n
 */
    for ( prev_pos = len = 0; src[len] != '\0'/* Not binary safe */; ++len )
    {
        if ( src[len] == ' ' )
        {
            if ( num == REQUEST_METHOD )
            {
                e_copymem(res->method, src+prev_pos, len - prev_pos);

                /* Which method */
                if ( e_str3cmp(res->method, "GET") )
                {
                    res->method_num = HTTP_GET;
                }
                else if ( e_str3cmp(res->method, "PUT"))
                {
                    res->method_num = HTTP_PUT;
                }
                else if ( e_str4cmp(res->method, "POST"))
                {
                    res->method_num = HTTP_POST;
                }
                else if ( e_str4cmp(res->method, "HEAD"))
                {
                    res->method_num = HTTP_HEAD;
                }
                else if ( e_str6cmp(res->method, "DELETE"))
                {
                    res->method_num = HTTP_DELETE;
                }
                else if ( e_str7cmp(res->method, "OPTIONS"))
                {
                    res->method_num = HTTP_OPTIONS;
                }
            }
            else if ( num == REQUEST_URL )
            {
                res->uri = e_substr(src + prev_pos, len - prev_pos);
            }

            num++;

            prev_pos = len + 1; /* Current is space so skip it. */
        }
        if ( src[len] == '\r' && src[len + 1] == '\n' )
        {
            if ( num == REQUEST_HTTP_VERSION )
            {
                res->version = e_substr(src + prev_pos, len - prev_pos);
                res->keep_alive = false;
                /* Get http version name */
                if ( e_str8cmp(res->version, E_HTTP_1_0) )
                {
                    res->version_num = HTTP_1_0;
                }
                else if ( e_str8cmp(res->version, E_HTTP_1_1) )
                {
                    res->version_num = HTTP_1_1;
                    res->keep_alive  = true;
                }
                else if ( e_str8cmp(res->version, E_HTTP_2_0) )
                {
                    res->version_num = HTTP_2_0;
                    res->keep_alive  = true;
                }
                prev_pos = len + 2;
                break;
            }
        }
    }
    
/**
 * @brief Introduction
 * Parse the HTTP request headers and body, for example:
 *
 * Content-Type: application/json;charset=UTF-8\r\n
 * Content-Length: 11\r\n
 * User-Agent: Exserver/1.1\r\n
 * \r\n
 * Hello World
 */
    colon_pos = 0;
    for ( len = prev_pos; src[len] != '\0'/* Not binary safe */; ++len )
    {
        if ( src[len] == ':' && !colon_pos )
        {
            colon_pos = len;
        }

        if ( (src[len] == '\r' && src[len + 1] == '\n'
            && src[len + 2] != '\r' && src[len + 3] != '\n')
            && colon_pos )
        {
            header = INIT_HTTP_HEADER();
            HTTP_HEADER_KEY_P(header) = e_substr(src + prev_pos, colon_pos - prev_pos);
            colon_pos++;
            E_SKIP_SPACE(src, colon_pos);
            HTTP_HEADER_VAL_P(header) = e_substr(src + colon_pos, len - colon_pos);

            /* Get the Connection header */
            if ( e_str14ncmp(HTTP_HEADER_KEY_P(header), E_LENGTH) )
            {
                res->body_length = strtoul(HTTP_HEADER_VAL_P(header), NULL, 10);
            }
            else if (e_str12ncmp(HTTP_HEADER_KEY_P(header), E_CTYPE ))
            {
                res->ctype = HTTP_HEADER_VAL_P(header);
                
                if ( e_memcmp(res->ctype, E_STRL("application/json")) == 0 )
                {
                    res->content_type = CTYPE_APPLICATION_JSON;
                }
                else if ( e_memcmp(res->ctype, E_STRL("multipart/form-data")) == 0 )
                {
                    res->content_type = CTYPE_FORM_DATA;
                }
                else if ( e_memcmp(res->ctype, E_STRL("application/x-www-form-urlencoded")) == 0 )
                {
                    res->content_type = CTYPE_MULTIPART;
                }
                else { res->content_type = CTYPE_UNKNOWN; }
            }
            else if (e_str5ncmp(HTTP_HEADER_KEY_P(header), E_RANGE))
            {
                res->range = HTTP_HEADER_VAL_P(header);
            }
            else if ( e_str10ncmp(HTTP_HEADER_KEY_P(header), E_CONNECT) )
            {
                if ( e_str10ncmp(HTTP_HEADER_VAL_P(header), E_KEEP) )
                {
                    res->keep_alive = true;
                }
                else
                {
                    res->keep_alive = false;
                }
            }
            /* Push into list */
            list_push_header(headers, header);
            prev_pos = len + 2;
            colon_pos = 0;
        }

        if ( (src[len] == '\r' && src[len + 1] == '\n' && src[len + 2] == '\r' && src[len + 3] == '\n') )
        {
/**
 * @brief Introduction
 * Before HTTP Request Body, There is the last Request Header need to be add to the QUEUE_LIST
 */
            header = INIT_HTTP_HEADER();
            HTTP_HEADER_KEY_P(header) = e_substr(src + prev_pos, colon_pos - prev_pos);
            colon_pos++;
            E_SKIP_SPACE(src, colon_pos);
            HTTP_HEADER_VAL_P(header) = e_substr(src + colon_pos, len - colon_pos);
            list_push_header(headers, header);

            /* Get the Connection header */
            if ( e_str14ncmp(HTTP_HEADER_KEY_P(header), E_LENGTH) )
            {
                res->body_length = strtoul(HTTP_HEADER_VAL_P(header), NULL, 10);
            }
            else if (e_str12ncmp(HTTP_HEADER_KEY_P(header), E_CTYPE ))
            {
                res->ctype = HTTP_HEADER_VAL_P(header);
    
                if ( e_memcmp(res->ctype, E_STRL("application/json")) == 0 )
                {
                    res->content_type = CTYPE_APPLICATION_JSON;
                }
                else if ( e_memcmp(res->ctype, E_STRL("multipart/form-data")) == 0 )
                {
                    res->content_type = CTYPE_FORM_DATA;
                }
                else if ( e_memcmp(res->ctype, E_STRL("application/x-www-form-urlencoded")) == 0 )
                {
                    res->content_type = CTYPE_MULTIPART;
                }
                else { res->content_type = CTYPE_UNKNOWN; }
            }
            else if (e_str5ncmp(HTTP_HEADER_KEY_P(header), E_RANGE))
            {
                res->range = HTTP_HEADER_VAL_P(header);
            }
            else if ( e_str10ncmp(HTTP_HEADER_KEY_P(header), E_CONNECT) )
            {
                if ( e_str10ncmp(HTTP_HEADER_VAL_P(header), E_KEEP) )
                {
                    res->keep_alive = true;
                }
                else
                {
                    res->keep_alive = false;
                }
            }
/**
 * @brief Introduction
 * Check whether the given data is valid or not.
 * if invalid, put into the trash can other than return
 */
            len += 4;
            /* body data */
            if ( !res->method_num ||
                (res->body_length > (src_len - len))
            )
            {
                trash_http_request(res);
                return NULL;
            }
            if ( res->body_length)
                res->body = e_substr(src + len, res->body_length);
/**
 * @brief Introduction
 * The next start parsing pos was current position plus the len and body length
 */
            *next_pos += len + res->body_length;

            break;
        }
    }
    return res;
}

void http_server_func(int fd, int signo, int event_fd)
{
/**
 * @brief Introduction
 * This function will dispatch all newly connected client to the different
 * worker process
 */
    int     client_fd;
again:
    client_fd = socket_accept(fd, NULL, NULL);
    if ( client_fd == -1 )
    {
        if ( errno == EAGAIN || errno == EWOULDBLOCK )
            goto again;
        else
        {
            logger(LOG_ERROR, "Socket accept wrong;" ERROR_CODE_FMT, ERROR_CODE_STR);
            return ;
        }
    }
    
    close(client_fd);
}

inline
void http_server_init()
{
    int        event_fd, fd;
    event_fd = event_init();
/**
 * @brief Introduction
 * Create the socket which you want to listen
 */
    fd = socket_create(NULL, 9898);
    socket_listen(fd, 10000);
/**
 * @brief Introduction
 * Add the newly created socket to the event listen loop
 */
    event_add(event_fd, fd);
    event_loop(event_fd, http_server_func);
}

#if DEBUG_MODE
#pragma clang diagnostic pop
#endif