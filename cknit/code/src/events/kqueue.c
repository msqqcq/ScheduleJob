/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <log.h>
#include <kqueue.h>

#ifndef __linux__

inline
int event_init()
{
/**
 * @brief Introduction
 * Create kqueue file descriptor and return
 */
    int fd;
    fd = kqueue();
    if ( fd == -1 )
    {
        logger(LOG_ERROR, "Create kqueue failed," ERROR_CODE_FMT, ERROR_CODE_STR);
        return -1;
    }
    return fd;
}

inline
int event_add(int fd, int sd)
{
/**
 * @brief Introduction
 * Add the second parameter to the first parameter's fd event loop
 */
    int                     re;
    struct kevent           ev;
    struct timespec         ts;
    memzero(&ev, sizeof(ev));
    memzero(&ts, sizeof(ts));
    
    ts.tv_sec = ts.tv_nsec = 0;
    EV_SET(&ev, sd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    re = kevent(fd, &ev, 1, NULL, 0, &ts);
    if ( re == -1 )
    {
        logger(LOG_ERROR, "Add fd %d to kqueue: %d list failed," ERROR_CODE_FMT, sd, fd, ERROR_CODE_STR);
        return -1;
    }
    
    return 1;
}

inline
int event_delete(int fd, int sd)
{
/**
 * @brief Introduction
 * Delete the sd from the fd
 */
    int                     re;
    struct kevent           ev;
    struct timespec         ts;
    memzero(&ev, sizeof(ev));
    memzero(&ts, sizeof(ts));
    
    ts.tv_sec = ts.tv_nsec = 0;
    EV_SET(&ev, sd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    re = kevent(fd, &ev, 1, NULL, 0, &ts);
    if ( re == -1 )
    {
        if ( errno == ENOENT ) return 0;
        logger(LOG_ERROR, "Remove fd %d from kqueue: %d list failed," ERROR_CODE_FMT, sd, fd, ERROR_CODE_STR);
        return -1;
    }
    
    return 1;
}

inline
int event_enable_write(int fd, int sd)
{
/**
 * @brief Introduction
 * Enable fd's write event listening
 */
    int                     re;
    struct kevent           ev;
    struct timespec         ts;
    memzero(&ev, sizeof(ev));
    memzero(&ts, sizeof(ts));
    
    ts.tv_sec = ts.tv_nsec = 0;
    EV_SET(&ev, sd, EVFILT_WRITE, EV_ADD|EV_ENABLE, 0, 0, NULL);
    re = kevent(fd, &ev, 1, NULL, 0, &ts);
    if ( re == -1 )
    {
        if ( errno == ENOENT ) return 0;
        logger(LOG_ERROR, "Add write fd %d to kqueue: %d list failed," ERROR_CODE_FMT, sd, fd, ERROR_CODE_STR);
        return -1;
    }
    
    return 1;
}

inline
int event_disable_write(int fd, int sd)
{
/**
 * @brief Introduction
 * Disble the fd's write event
 */
    int                     re;
    struct kevent           ev;
    struct timespec         ts;
    memzero(&ev, sizeof(ev));
    memzero(&ts, sizeof(ts));
    
    ts.tv_sec = ts.tv_nsec = 0;
    EV_SET(&ev, sd, EVFILT_WRITE, EV_DELETE | EV_DISABLE, 0, 0, NULL);
    re = kevent(fd, &ev, 1, NULL, 0, &ts);
    if ( re == -1 )
    {
        if ( errno == ENOENT ) return 0;
        logger(LOG_ERROR, "Delete fd %d from kqueue: %d list failed," ERROR_CODE_FMT, sd, fd, ERROR_CODE_STR);
        return -1;
    }
    
    return 1;
}

inline
void event_loop(int fd, void (*func)(int, int, int))
{
/**
 * @brief NOTICE
 *  void(*func)(int, int, int)
 *     first parameter  of type is: The IO coming file descriptor
 *     second parameter of type is: The IO event name: READ or Write
 *     third parameter  of type is: The event file descriptor
 */
    int             i,    j;
    struct timespec ts;
    struct kevent   events[EVENT_SIZE];
    
    memzero(&ts,    sizeof(ts));
    
    ts.tv_sec  = EVENT_TIMEOUT;
    ts.tv_nsec = 0;
    
    while (true)
    {
        memzero(events, sizeof(events));
        
        i = kevent( fd, NULL, 0, events, EVENT_SIZE, &ts );
    
        if ( i == -1 )
        {
            logger(LOG_ERROR, "Kevent wrong," ERROR_CODE_FMT, ERROR_CODE_STR);
            continue;
        }
        
        for (j = 0; j < i; j++)
        {
            if ( events[j].flags & ( EV_ERROR | EV_EOF ))
            {
                event_delete(fd, (int)events[j].ident);
                close( ( int )events[j].ident );
                continue;
            }
            
            if (   events[j].filter == EVFILT_READ
                || events[j].filter == EVFILT_WRITE )
            {
                func((int)events[j].ident, events[j].filter, fd);
            }
        }
    }
    
    logger(LOG_ERROR, "Loop wrong, never be here!" ERROR_CODE_FMT, ERROR_CODE_STR);
}

#endif