/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef LISTS_HTTP_REQUEST_H
#define LISTS_HTTP_REQUEST_H

#if DEBUG_MODE
    #pragma clang diagnostic push
    #pragma ide diagnostic ignored "OCUnusedMacroInspection"
    #pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#endif

#include <log.h>
#include <fcntl.h>
#include <list.h>
#include <unistd.h>

typedef struct _HTTP_HEADER
{
    char *key;
    char *val;
} HTTP_HEADER;

enum HTTP_HEADER_LINE{
    REQUEST_METHOD = 0,
    REQUEST_URL,
    REQUEST_HTTP_VERSION
};

enum HTTP_VERSION_NUM{
    HTTP_1_0 = 0,
    HTTP_1_1,
    HTTP_2_0
};

enum HTTP_CONTENT_TYPE {
CTYPE_FORM_DATA = 0, CTYPE_MULTIPART = 2/*x-www-form-urlencoded*/,
CTYPE_APPLICATION_JSON = 4, CTYPE_UNKNOWN = 8 };

enum HTTP_METHOD_NUM{
    HTTP_GET = 1,
    HTTP_PUT,
    HTTP_POST,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS
};

#define E_HOST     "Host"
#define E_RANGE    "Range"
#define E_TAG      "ETag"
#define E_CTYPE    "Content-Type"
#define E_KEEP     "keep-alive"
#define E_LENGTH   "Content-Length"
#define E_CONNECT  "Connection"
#define E_HTTP_1_0 "HTTP/1.0"
#define E_HTTP_1_1 "HTTP/1.1"
#define E_HTTP_2_0 "HTTP/2.0"

typedef struct _HTTP_REQUEST
{
    char  keep_alive;    /* keep alive or not */
    char  method_num;    /* HTTP_GET or HTTP_PUT ... */
    char  version_num;   /* HTTP_1_0 or HTTP_1_1 or HTTP_2_0 */
    char  content_type;  /* Content-Type */
    int   fd;            /* Socket file descriptor */
    char *ctype;         /* Raw Content-Type info. */
    char *boundary;      /* When multipart/form-data boundary is set */
    char *uri;           /* HTTP request url: such as /index.php or /index.html and so on ... */
    char *version;       /* HTTP/1.0 or HTTP/1.1 */
    char *range;         /* Range header */
    QUEUE_LIST *headers; /* HTTP request headers */
    char *body;          /* HTTP request body */
    unsigned long body_length;   /* HTTP request body length */
    char  method[8];     /* GET | PUT | POST | DELETE | HEAD | OPTIONS and so on.*/
} HTTP_REQUEST;

/* Declarations for some universal functions */
HTTP_HEADER  *INIT_HTTP_HEADER();
HTTP_REQUEST *INIT_HTTP_REQUEST();
void trash_http_header(void *data/* data's type is HTTP_HEADER */);
void trash_http_request(HTTP_REQUEST *);

/* Some global variables */
#define GLOBAL_VARS
typedef void(*HTTP_CALLBACK)(void *);

/* short way to append data */
#define list_append_header(l, v)  list_append(l, v, trash_http_header)
#define list_push_header(l, v)    list_push(l, v, trash_http_header)

#define HTTP_HEADER_KEY(v) (v).key
#define HTTP_HEADER_KEY_P(v) HTTP_HEADER_KEY(*(v))
#define HTTP_HEADER_VAL(v) (v).val
#define HTTP_HEADER_VAL_P(v) HTTP_HEADER_VAL(*(v))

#define E_SKIP_SPACE(src, i) do {\
    for (; src[i] == ' '; i++) ;} while(0);

/* Following part is the HTTP request parsing functions */

HTTP_REQUEST *
http_stream_parse(char *src, unsigned long src_len, unsigned long *next_pos);

/* void http_callback(int fd, int signo, int event_fd); */
/* void http_worker_func(int worker_index, void (*callback)(void *));*/
void http_worker_init(int num, void(*callback)(void *));
/* void http_server_func(int fd, int signo, int event_fd); */
void http_server_init();

#if DEBUG_MODE
    #pragma clang diagnostic pop
#endif

#endif /* LISTS_HTTP_REQUEST_H */