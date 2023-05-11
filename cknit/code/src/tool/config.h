/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef LISTS_CONFIG_H
#define LISTS_CONFIG_H

#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <signal.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define true  1
#define false 0

#define TRUE  1
#define FALSE 0
#define elif else if

#define DEBUG_MODE true

#define EVENT_SIZE      100
#define BUFFER_SIZE     256
#define EVENT_TIMEOUT   120
#define MAX_PROCESS_NUM 256

#define INFO_LOG_NAME  "/var/log/cknit/info.log"
#define NOTI_LOG_NAME  "/var/log/cknit/notice.log"
#define WARN_LOG_NAME  "/var/log/cknit/warning.log"
#define ERRO_LOG_NAME  "/var/log/cknit/error.log"
#define TASK_LOG_NAME "/var/log/cknit/task.log"
#define OPER_LOG_NAME "/var/log/cknit/operation.log"

#define memzero(v, l) memset(v, 0, l)
#ifndef e_memfree
    #define e_memfree free
#endif


#endif /* LISTS_CONFIG_H */
