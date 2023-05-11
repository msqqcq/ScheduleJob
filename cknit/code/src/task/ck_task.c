/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <ck_task.h>
#include <http_request.h>
#include <fc_json.h>
#include <fc_tool.h>
#include <signal.h>
#include <log.h>
#include <fc_query_string.h>

/**
 * @brief Introduction
 * This thread works for HTTP UI events
 */
void http_callback(int fd, int signo, int event_fd)
{
    int                 cfd;
    HTTP_REQUEST        *req;
    long                blen,    n;
    unsigned long       nlen;
    long     long       tstatus;
    FAST_CJSON          *res,   *tjson,  *rve;
    char                *buff,  *r,      *qr;
    CJSON_tasks         *task,  *ttsk;
    CQUERY_task         *qtask;
    CQUERY              *query;
    CJSON_taskId        *taskId;
    CSTRING             *req_buff;
    
    if ( fd == server_fd )
    {
        cfd = socket_accept( fd, NULL, NULL );
        event_add(event_fd, cfd);
    }
    else
    {
        cfd = fd;
        req_buff = new_cstring();
        
read_buff:
        n    = 0;
        blen = 0;
        
        buff = socket_read(cfd, &blen);
        new_cstring_add_string(req_buff, buff, blen);
        
        if ( !buff || blen == -1 ) {
            new_cstring_free(req_buff);
            close(cfd);
            return ;
        }
        req  = http_stream_parse( req_buff->s, ( unsigned long )blen, ( unsigned long * )&n );
        if ( req_buff->l && !req )
        {
            goto read_buff;
        } else {
            new_cstring_free(req_buff);
        }
        e_memfree(buff);
        if ( !req ) {
            return ;
        }
        if ( *(req->uri) == '/' && *(req->uri + 1) == '\0' )
        {
            r    = NULL;
            buff = e_data_from_file("/etc/cknit/default_api.exjson", &n);
            nlen = http_cknit_response( &r, 200, E_STRL("OK"), buff, ( unsigned long )n, req->keep_alive);
            socket_write(cfd, r, nlen);
            e_memfree(buff);
            e_memfree(r);
        }
        else if ( req->method_num == HTTP_GET && e_memcmp(req->uri, E_STRL("/monitors")) == 0 )
        {
/**
 * @brief Introduction
 * if to get the Task list, response it with the JSON response.
 */
            qtask= NULL;
            qr   = strchr(req->uri, '?');
            if ( qr )
            {
                query = new_cquery_from_string(qr);
                qtask = NEW_CQUERY_task_FROM_CQUERY(query);
            }
            
            if ( qtask && qtask->__oth )
            {
                r    = NULL;
                TRASH_CQUERY_task(qtask);
                nlen = http_cknit_response(
                    &r, 424, E_STRL("Validation Error"),
                    E_STRL("{\"message\":\"Query String only support `from` var.\",\"code\":\"false\"}"),
                    req->keep_alive
                );
                socket_write(cfd, r, nlen);
                e_memfree(r);
                goto trash_close;
            }
            
            if ( qtask && qtask->from )
            {
                tstatus = strtol(qtask->from->val, NULL, 10);
            }
            else
            {
                tstatus = 1;
            }
            
            rve = cjson_copy_size(tasks, (int)tstatus, (int)(tstatus + task_num) );
            buff = encode_cjson(rve, FALSE);
            
            TRASH_CQUERY_task(qtask);
            fcl_list_destroy(rve);
            
            r    = NULL;
            nlen = http_cknit_response(
                &r, 200, E_STRL("OK"), buff, strlen(buff),
                req->keep_alive
            );
            socket_write(cfd, r, sizeof(char) * nlen);
            e_memfree(r);
            e_memfree(buff);
        }
        else
        if ( e_memcmp(req->uri, E_STRL("/monitors")) == 0 && strlen(req->uri) == 9 )
        {
           if ( req->method_num == HTTP_POST )
            {
/**
 * @brief Introduction
 * When JSON POST to add Task, do the add job
 */
                res = decode_cjson(req->body);
                if ( !res ) {
                    r    = NULL;
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Post body need JSON.\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                
                task = NEW_CJSON_tasks_FROM_CJSON(res);
                if ( !task || task->oth )
                {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    TRASH_CJSON_tasks(task);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Invalid JSON data\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                if ( task->id )
                {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    TRASH_CJSON_tasks(task);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Task data canot have id arg.\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                if ( !task->command ||
                    ((char *)CJSON_VAL_P(task->command))[0] == '\0' )
                {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    TRASH_CJSON_tasks(task);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Invalid command parameter\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
    
                if ( !task->period ||
                     ((char *)CJSON_VAL_P(task->period))[0] == '\0' )
                {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    TRASH_CJSON_tasks(task);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Invalid period parameter\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                
                if ( !task->status ) {
                    cjson_object_add_int(res, E_STRL(STATUS), TASK_STOP);
                }
                if ( tasks->tail )
                {
                    tjson   = TAIL_FAST_CJSON( tasks );
                    tstatus = *( long long * )cjson_find_key( tjson, E_STRL( "id" ) );
                } else tstatus = 0;
                cjson_object_add_int(res, E_STRL("id"), tstatus + 1);
                cjson_array_add_cjson(tasks, res, CJSON_OBJECT);

                logger(LOG_OPRA,
                    "Operation: [ADD]  command: [%s], period: [%s], status:[%s], id:[%ld]\n",
                       (char *)task->command->val, (char *)task->period->val,
                       ( task->status && *(long long *)task->status->val == 1) ? "ON" : "OFF",
                       (tstatus + 1)
                );
                TRASH_CJSON_tasks(task);
/**
 * @brief NOTICE
 * After add task successfully, dump data to the ckdb file if exists
 */
                if ( task_data[0] != '\0' )
                {
                    e_write_to_file(task_data, "%s", encode_cjson(tasks, FALSE));
                }
                r    = NULL;
                nlen = http_cknit_response(
                    &r, 200, E_STRL("OK"),
                    E_STRL("{\"message\":\"Success\",\"code\":\"true\",\"operation\":\"ADD\"}"),
                    req->keep_alive
                );
                socket_write(cfd, r, nlen);
                e_memfree(r);
            }
            else
            if ( req->method_num == HTTP_PUT )
            {
/**
 * @brief Introduction
 * Modify existing task
 */
                res  = decode_cjson(req->body);
                if ( !res ) {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Post body need JSON.\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                tstatus = *(long long *)cjson_find_key(res, E_STRL("id"));
                if ( !blen ) {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Need id key parameter\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                rve = cjson_find_key(res, E_STRL("data"));
                if ( !rve ) {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Need data parameter\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                
                task = NEW_CJSON_tasks_FROM_CJSON(rve);
                if ( !task || task->oth )
                {
                    r    = NULL;
                    TRASH_CJSON_tasks(task);
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Invalid JSON request data\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                if ( task->id )
                {
                    r    = NULL;
                    TRASH_CJSON_tasks(task);
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Modify request can't contain id arg\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }

                tjson = list_find_id(tasks, tstatus);
                if ( !tjson )
                {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"No data with the given id\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                ttsk = NEW_CJSON_tasks_FROM_CJSON(tjson);
                if ( task->status )
                {
                    e_memfree(ttsk->status->val);
                    ttsk->status->val = malloc(sizeof(long long));
                    e_copymem(ttsk->status->val, task->status->val, sizeof(long long));
                }
                if ( task->command )
                {
                    e_memfree(ttsk->command->val);
                    ttsk->command->val = new_cjson_string(task->command->val, strlen(task->command->val)+ 1);
                    e_copymem(ttsk->command->val, task->command->val, strlen(task->command->val));
                }
                if ( task->period )
                {
                    e_memfree(ttsk->period->val);
                    ttsk->period->val = new_cjson_string(task->period->val, strlen(task->period->val)+ 1);
                    e_copymem(ttsk->period->val, task->period->val, strlen(task->period->val));
                }
/**
 * @brief Introduction
 * Need to be modified
 */
                logger(LOG_OPRA,
                       "Operation: [MODIFY]  command: [%s->%s], period: [%s->%s], status:[%s->%s], id:[%ld]\n",
                       ttsk->command ? (char *)ttsk->command->val : "NULL", task->command ? (char *)task->command->val : "NULL",
                       ttsk->period ? (char *)ttsk->period->val : "NULL",  task->period ? (char *)task->period->val : "NULL",
                       ttsk->status ? ( (*(long long *)ttsk->status->val == 1) ? "ON" : "OFF") : "OFF",
                       task->status ? ( ( task->status && *(long long *)task->status->val == 1) ? "ON" : "OFF" ) : "OFF",
                       (*(long long *)ttsk->id->val)
                );
                
                TRASH_CJSON_tasks(task);
                TRASH_CJSON_tasks(ttsk);
                fast_cjson_destroy(res);
/**
 * @brief NOTICE
 * After modify task successfully, dump the data to ckdb if exists
 */
                if ( task_data[0] != '\0' )
                {
                    e_write_to_file(task_data, "%s", encode_cjson(tasks, FALSE));
                }
                
                r    = NULL;
                nlen = http_cknit_response(
                    &r, 200, E_STRL("OK"),
                    E_STRL("{\"message\":\"Success\",\"code\":\"true\",\"operation\":\"MODIFY\"}"),
                    req->keep_alive
                );
                socket_write(cfd, r, nlen);
                e_memfree(r);
            }
            else
            if ( req->method_num == HTTP_DELETE )
            {
/**
 * @brief Introduction
 * Delete task with the given id
 */
                res  = decode_cjson(req->body);
                if ( !res ) {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Post body need valid JSON.\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                
                taskId = NEW_CJSON_taskId_FROM_CJSON(res);
                if ( !taskId || taskId->oth )
                {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    TRASH_CJSON_taskId(taskId);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Invalid JSON request data\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                
                if ( !taskId->id )
                {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Need id key parameter\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
    
                if (*(long long *)taskId->id->val <= 0) {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 424, E_STRL("Validation Error"),
                        E_STRL("{\"message\":\"Invalid id arg\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                
                if ( list_delete_id(tasks, *(long long *)taskId->id->val) == -1 )
                {
                    r    = NULL;
                    fast_cjson_destroy(res);
                    nlen = http_cknit_response(
                        &r, 404, E_STRL("Not Found"),
                        E_STRL("{\"message\":\"Given id not exist.\",\"code\":\"false\"}"),
                        req->keep_alive
                    );
                    socket_write(cfd, r, nlen);
                    e_memfree(r);
                    goto trash_close;
                }
                
                fast_cjson_destroy(res);
    
                logger(LOG_OPRA,
                       "Operation: [DELETE]  id:[%ld]\n",
                       *(long long *)taskId->id
                );
/**
 * @brief NOTICE
 * After DELETE task successfully, dump the data to ckdb if exists
 */
                if ( task_data[0] != '\0' )
                {
                    e_write_to_file(task_data, "%s", encode_cjson(tasks, FALSE));
                }
                TRASH_CJSON_taskId(taskId);
                r    = NULL;
                nlen = http_cknit_response(
                    &r, 200, E_STRL("OK"),
                    E_STRL("{\"message\":\"Success.\",\"code\":true,\"operation\":\"DELETE\"}"),
                    req->keep_alive
                );
                socket_write(cfd, r, nlen);
                e_memfree(r);
            }
        }
        else
        {
/**
 * @brief Introduction
 * Other requests response Invalid Request
 */
            r    = NULL;
            nlen = http_cknit_response(
                &r, 404, E_STRL("Not Found"),
                E_STRL("{\"message\":\"Invalid Request\",\"code\":\"false\"}"),
                req->keep_alive
            );
            socket_write(cfd, r, nlen);
            e_memfree(r);
        }

trash_close:

        trash_http_request(req);

        if ( !req->keep_alive ) {
            close(cfd);
        }
    }
}

void *http_thread(void *d)
{
    int event_fd;
    event_fd  = event_init();
    server_fd = socket_create(NULL, 9898);
    socket_listen(server_fd, 1000);
    event_add(event_fd, server_fd);
    event_loop(event_fd, http_callback);
    return NULL;
}

/**
 * @brief NOTICE
 * Copy the FastCJSON structure to another list
 * and then destory it after used.
 */
FAST_CJSON *cjson_copy_size(FAST_CJSON *src, int from, int to)
{
    FCL_LIST   *ptr;
    FAST_CJSON *result;
    int         rsize;
    FAST_CJSON *fcjs;
    void       *v;
    FCL_NODE   *fnode;
    CJSON_NODE *cnode;
    
    rsize = to - from;
    result = new_fast_cjson(CJSON_ARRAY);
    
    if ( !src || rsize <= 0 ) { return result ; }
    
    fnode = FCL_LIST_HEAD_P(src);
    
    if ( from < FCL_LIST_NUM_P(src) )
    {
        while ( --from )
        {
            if ( fnode )
                fnode = FCL_NODE_NEXT_P(fnode);
            else {
                break;
            }
        }
    }
    else if ( from > FCL_LIST_NUM_P(src) )
    {
        return result;
    }
    else if ( from == FCL_LIST_NUM_P(src) )
    {
        fnode = FCL_LIST_TAIL_P(src);
    }
    
    if ( !fnode ) { return result; }
    
    
    for ( ; fnode != NULL ; fnode = FCL_NODE_NEXT_P(fnode) )
    {
        if ( rsize-- )
        {
            cnode = FCL_NODE_DATA_P(fnode);
            ptr   = CJSON_VAL_P(cnode);
    
            fcjs  = new_fast_cjson(CJSON_OBJECT);
    
            CJSON_ARRAY_FOREACH(ptr, v) {
                if ( CJSON_TYPE_P(cnode) & ( CJSON_BOOL | CJSON_INT ) )
                {
                    cjson_object_add_int_tag(fcjs, cnode->key, strlen(cnode->key), *(long long *)cnode->val, CJSON_TYPE_P(cnode));
                }
                else if ( CJSON_TYPE_P(cnode) == CJSON_STRING)
                {
                    cjson_object_add_string(fcjs, cnode->key, strlen(cnode->key), cnode->val, strlen(cnode->val));
                }
            } CJSON_ARRAY_FOREACH_END();
    
            cjson_array_add_cjson(result, fcjs, CJSON_OBJECT);
        } else break;
    }
    
    return result;
}

void sig_callback(int signo, siginfo_t *siginfo, void *context)
{
    switch (signo)
    {
        case SIGUSR1:
/**
 * @brief NOTICE
 * Some thing can be done like reload the config file
 */
            break;
        case SIGUSR2:
/**
 * @brief NOTICE
 * When SIGUSR2 coming, exit the current process
 */
            exit(0);
        default:break;
    }
}

/**
 * @brief Introduction
 * This thread for task health status check.
 */
void task_thread()
{

}

inline
void trash_file_fp(void *v)
{
    if(!v) return ;
    pclose((FILE *)v);
}

inline
void trash_file_exception(void *v)
{
    if ( !v ) return ;
    HTTP_HEADER *t = (HTTP_HEADER *)v;
    e_memfree(HTTP_HEADER_KEY_P(t));
    e_memfree(HTTP_HEADER_VAL_P(t));
    e_memfree(v);
}

inline
void task_fork(char *command)
{
    FILE      *fp;
    size_t     blen;
    char  buff[BUFFER_SIZE] = {0};
    fp = popen(command, "r");
    logger(LOG_TASK, "Task: [%s] running.\n", command);
    blen = fread(buff, sizeof(char), sizeof(buff), fp);
    if (blen)
    {
        logger(LOG_TASK, "Task: [%s] error msg: %s.\n", command, buff);
        logger(LOG_ERROR, "Task[%s] run faild:%s\n", command, buff);
        pclose(fp);
    }
}

/**
 * @brief Introduction
 * This thread works for Jobs when timer is over.
 */
void main_thread()
{
    long long         tstatus;
    long              re,    pd,  fsize;
    CJSON_stask      *stask;
    char             *conf,  *period, *command;
    FCL_NODE         *node,  *each;
    FAST_CJSON       *tjson;
    sigset_t          sset;
    struct sigaction  sig;
    struct timeval     tv;
    
    node  = NULL;
    each  = NULL;
    tjson = NULL;
    
/**
 * @brief NOTICE
 * Create signal callback when signal coming
 */
    e_memzero(&sig, sizeof(struct sigaction));
    sig.sa_sigaction = sig_callback;
    sig.sa_flags = SA_SIGINFO;
    
    sigemptyset(&sig.sa_mask);
    sigaddset(&sig.sa_mask, SIGUSR1);
    sigaction(SIGUSR1, &sig, NULL);
    
    sigemptyset(&sig.sa_mask);
    sigaddset(&sig.sa_mask, SIGUSR2);
    sigaction(SIGUSR2, &sig, NULL);
    
    sigfillset(&sset);
    sigdelset(&sset, SIGUSR1);
    sigdelset(&sset, SIGUSR2);
    sigprocmask(SIG_BLOCK, &sset, NULL);
 
/**
 * @brief Introduction
 * To find whether the cknit is running or not
 */
    conf = e_data_from_file(PIDFILE, NULL);
    if (conf)
    {
        errno = 0;
        pd    = strtol( conf, NULL, 10);
        if ( pd != 0 )
        {
            re = kill( (int)pd, 0);
            if ( re == 0 || ( re != -1 || errno == EPERM ) )
            {
                fprintf( stderr, "cknit:[PID: %ld] has been started before.\n", pd );
                return;
            }
        }
    }
    
    pthread_mutex_init(&status, NULL);
   
/**
 * @brief Introduction
 * Read all system config file
 */
    conf  = e_data_from_file(SYS_CONF_FILE, NULL);
    if (!conf)
    {
        logger(LOG_ERROR, "Open file:%s failed.\n", SYS_CONF_FILE);
        return ;
    }
    system_configs = decode_cjson(conf);
    if ( !system_configs ) {
        logger(LOG_ERROR, "Syntax wrong: %s\n", SYS_CONF_FILE);
        return ;
    }
    e_memfree(conf);
    
    e_system = cjson_find_key(system_configs, E_STRL(SYSTEM));
    if ( !e_system )
    {
        logger(LOG_ERROR, "%s need system node.\n", SYS_CONF_FILE);
        return ;
    }
/**
 * @brief Introduction
 * Get the task config file and load it to the global memory
 * < tasks >
 */
    stask = NEW_CJSON_stask_FROM_CJSON(e_system);
    if ( !stask->task_conf || stask->task_conf->val == '\0' )
    {
        logger(LOG_ERROR, "%s file need valid %s node\n", SYS_CONF_FILE, TASK_CONF);
        return ;
    }
    
/**
 * @brief NOTICE
 * First find the data directory is exists and the ckdb file is exists
 * if not read the task data from the task_conf.
 */
    if ( stask->data )
    {
        fsize = strlen((char *)stask->data->val);
        e_memzero(task_data, sizeof(task_data));
        e_copymem(task_data, stask->data->val, fsize);
    
        if ( access(task_data, F_OK | W_OK | R_OK) == -1 )
        {
            logger(LOG_ERROR, "Open directory: %s wrong, error msg: %s\n", stask->data->val, strerror(errno));
            return ;
        }
        
        e_copymem(task_data + fsize, E_STRL("/ckdb"));
        
        fsize = 0;
        conf  = e_data_from_file(task_data, &fsize);
        if ( fsize == 0 )
        {
            conf  = e_data_from_file(stask->task_conf->val, NULL);
        }
    } else {
        conf  = e_data_from_file(stask->task_conf->val, NULL);
    }
    
    tasks = decode_cjson(conf);
    e_memfree(conf);
    if(!tasks)
    {
        logger(LOG_ERROR, "Default task file: %s format mistake, Invalid Exjson.\n", stask->task_conf->val);
        return ;
    }
/**
 * @brief Introduction
 * From now on tasks can't be read again.
 */
    re = stask->daemon ? *(long long *)stask->daemon->val : 0;
    if ( re ) {
        if ( fork() != 0 ) exit(0);
    }
    
    TRASH_CJSON_stask(stask);
/**
 * @brief Introduction
 * Write PID to file
 */
    e_write_to_file(PIDFILE, "%d", getpid());
    
/**
 * @brief Introduction
 * If not set the default ID or status field
 * cknit will automatically set it, ID will be auto-increment
 * and status will set to TASK_STOP
 */
    re = 1;
    CJSON_NODE *v;
    FCL_LIST_FOREACH_HEAD(tasks, node) {
        tjson = CJSON_VAL_P((CJSON_NODE *)FCL_NODE_DATA_P(node));
        
        if ( !cjson_find_key(tjson, E_STRL(STATUS)) )
        {
            cjson_object_add_int(tjson, E_STRL(STATUS), TASK_STOP);
        }
        
        if ( !cjson_find_key(tjson, E_STRL("id")) )
        {
            cjson_object_add_int(tjson, E_STRL("id"), re++);
        }
    } FCL_LIST_FOREACH_END();
    
/**
 * @brief Introduction
 * Listen the http requests
 */
    pthread_t pid;
    pthread_create(&pid, NULL, http_thread, NULL);
    
    while ( 1 )
    {
        tv.tv_sec  = 1;
        tv.tv_usec = 0;
        
        re = CK_TIMER( &tv );
        assert( re != -1 );
        
        if ( !pthread_mutex_trylock(&status) ) { continue; }
/**
 * @brief Introduction
 * Each time, need to loop all tasks, for on-time job.
 */
        FCL_LIST_FOREACH_HEAD(tasks, node) {
            tjson = CJSON_VAL_P((CJSON_NODE *)FCL_NODE_DATA_P(node));
            command = NULL;
            period  = NULL;
            tstatus = TASK_STOP;
            FCL_LIST_FOREACH_HEAD(tjson, each) {
                v = FCL_NODE_DATA_P(each);
                if ( e_memcmp(CJSON_KEY_P(v), E_STRL(COMMAND)) == 0 )
                {
                    command = CJSON_VAL_P(v);
                }
                elif ( e_memcmp(CJSON_KEY_P(v), E_STRL(PERIOD)) == 0 )
                {
                    period = CJSON_VAL_P(v);
                }
                elif ( e_memcmp(CJSON_KEY_P(v), E_STRL(STATUS)) == 0 )
                {
                    tstatus = *(long long *)CJSON_VAL_P(v);
                }
            } FCL_LIST_FOREACH_END();
            if ( period && command && e_match_time(period) && tstatus == TASK_ON )
            {
                task_fork(command);
            }
        } FCL_LIST_FOREACH_END();
        pthread_mutex_unlock(&status);
    }
}
