/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef LISTS_LOG_H
#define LISTS_LOG_H

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

enum {
    LOG_INFO = 0,
    LOG_NOTICE,
    LOG_WARNING,
    LOG_ERROR,
    LOG_TASK,
    LOG_OPRA
};

/**
 * @brief Notice
 * Not to invoke the function directly, instead using the logger(...) function
 * for the reason LOG_LEVEL control.
 */
void _logger(int type, char *str, ...);

#if DEBUG_MODE
    #define logger(type, str, ...) _logger(type, str, ##__VA_ARGS__)
#else
    #define logger(type, str, ...) do{\
        if (type > LOG_WARNING) {\
            _logger(type, str, ##__VA_ARGS__);\
        }} while(0);
#endif

#define ERROR_CODE_FMT "code: %d, msg: %s"
#define ERROR_CODE_STR  errno, strerror(errno)

#endif /* LISTS_LOG_H */
