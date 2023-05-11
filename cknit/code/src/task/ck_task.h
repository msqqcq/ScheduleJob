/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"


#ifndef CKNIT_CK_TASK_H
#define CKNIT_CK_TASK_H

#include <ck_timer.h>
#include <config.h>
#include <list.h>
#include <estring.h>
#include <http_socket.h>
#include <event.h>
#include <fc_json.h>
#include <help.h>
#include <pthread.h>

/**
 * @brief Introduction
 * Some global variables
 * tasks will automatically destroy, after successfully init the kernel process data
 * e_system was the cknit.exjson's Exjson data structure[lifestyle : whole app]
 */
#define GLOBAL_VARS
GLOBAL_VARS static pthread_mutex_t status;
/*
GLOBAL_VARS FAST_CJSON *tasks,    *e_system;
GLOBAL_VARS QUEUE_LIST *tasks_fp, *tasks_mistake, *jobs;
*/
GLOBAL_VARS static int  task_num = 5;
GLOBAL_VARS char        task_data[BUFFER_SIZE];
GLOBAL_VARS FAST_CJSON *system_configs, *e_system, *tasks;
GLOBAL_VARS int         server_fd;

void trash_file_exception(void *v);

#define TASK_UNSET -1
#define ID_UNSET   -1
enum TASK_STATUS {
    TASK_STOP = 0,
    TASK_ON = 1
};

/**
 * @brief Introduction
 * Some name macros
 */
#define COMMAND "command"
#define PERIOD  "period"
#define STATUS  "status"
#define OP_DOWN "op_down"
#define FOREVER "forever"

#define SYSTEM    "system"
#define DAEMON    "daemon"
#define TASK_CONF "task_conf"
#define PIDFILE   "/var/log/cknit/pid"
#define SYS_CONF_FILE "/etc/cknit/cknit.exjson"

/**
 * @brief Introduction
 * Some macros for loop the Exjson structure
 * v's type is EXJSON_V
 */
#define EXJSON_FOR_EACH_OBJECT(e, v) do { int i, j; EXJSON *node; if (!e) break; for ( i = 0; i < E_NUM_P(e); ++i ) { v = E_DATA_P(e) + i;
#define EXJSON_ARRAY_EACH(e, v) do { int i, j; EXJSON *node; for ( i = 0; i < E_NUM_P(e); ++i ) { v = node = EV_VALUE_P(E_DATA_P(e) + i);
#define EXJSON_ARRAY_EACH_END() }} while(0)
#define EXJSON_FOR_EACH_ARRAY(e, v) do { int i, j; EXJSON *node; for ( i = 0; i < E_NUM_P(e); ++i ) {node = EV_VALUE_P(E_DATA_P(e) + i); for ( j = 0; j < E_NUM_P(node); ++j ) { v = E_DATA_P( node ) + j;
#define EXJSON_FOR_EACH_ARRAY_END() }}} while(0)
#define EXJSON_FOR_EACH_OBJECT_END() }} while(0)

FAST_CJSON *cjson_copy_size(FAST_CJSON *src, int from, int to);

void  trash_file_fp(void *v);
void  task_fork(char *command);
void  main_thread();
void *http_thread(void *p);

#endif /* CKNIT_CK_TASK_H */

#pragma clang diagnostic pop