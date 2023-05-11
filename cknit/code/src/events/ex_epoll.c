/**
 * Copyright @2019 Exserver All Rights Reserved.
 */

#include <log.h>
#include <ex_epoll.h>
#include <config.h>
#include <estring.h>

#ifdef __linux__

inline int event_init()
{
    int fd;
    
    fd = epoll_create(1);
    if ( fd == -1 )
    {
        logger(LOG_ERROR, "Create epoll failed," ERROR_CODE_FMT, ERROR_CODE_STR);
        return -1;
    }
    return fd;
}

inline int event_add(int fd, int sd)
{
    int                 re;
    struct epoll_event  ev;
    e_memzero(&ev, sizeof(ev));
    
    ev.data.fd = sd;
    ev.events = EPOLLIN;
    
    re = epoll_ctl(fd, EPOLL_CTL_ADD, sd, &ev);
    if ( re == -1 )
    {
        logger(LOG_ERROR, "Add fd %d to epoll: %d list failed," ERROR_CODE_FMT, sd, fd, ERROR_CODE_STR);
        return -1;
    }
    return 1;
}

int event_enable_write(int fd, int sd)
{
    int                     re;
    struct epoll_event      ev;
    e_memzero(&ev, sizeof(ev));
    
    ev.data.fd = sd;
    ev.events = EPOLLIN | EPOLLOUT;
    
    re = epoll_ctl(fd, EPOLL_CTL_MOD, sd, &ev);
    if ( re == -1 )
    {
        logger(LOG_ERROR, "Add write fd %d to epoll: %d list failed," ERROR_CODE_FMT, sd, fd, ERROR_CODE_STR);
        return -1;
    }
    return 1;
}

int event_disable_write(int fd, int sd)
{
    int                     re;
    struct epoll_event      ev;
    e_memzero(&ev, sizeof(ev));
    
    ev.data.fd = sd;
    ev.events = EPOLLIN;
    
    re = epoll_ctl(fd, EPOLL_CTL_MOD, sd, &ev);
    if ( re == -1 )
    {
        logger(LOG_ERROR, "Delete fd %d from epoll: %d list failed," ERROR_CODE_FMT, sd, fd, ERROR_CODE_STR);
        return -1;
    }
    return 1;
}

inline int event_delete(int fd, int sd)
{
    int                 re;
    
    re = epoll_ctl(fd, EPOLL_CTL_DEL, sd, NULL);
    if ( re == -1 )
    {
        logger(LOG_ERROR, "Remove fd %d from epoll: %d list failed," ERROR_CODE_FMT, sd, fd, ERROR_CODE_STR);
        return -1;
    }
    return 1;
}

inline void event_loop(int fd, void (*func)(int, int, int))
{
    int                 en, i;
    struct epoll_event  event[EVENT_SIZE];
    
    while ( true )
    {
        e_memzero(event, sizeof(event));
        en = epoll_wait(fd, event, EVENT_SIZE, EVENT_TIMEOUT * 1000);
        
        if ( en == -1 )
        {
            logger(LOG_ERROR, "epoll_wait wrong," ERROR_CODE_FMT, ERROR_CODE_STR);
            continue;
        }
        
        if ( en )
        {
            for ( i = 0; i < en; i++ )
            {
                if ( event[i].events & ( EPOLLRDHUP ) )
                {
                    close(event[i].data.fd);
                }
                else if ( event[i].events & ( EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR ) )
                {
                    func(event[i].data.fd, event[i].events, fd);
                }
            }
        }
    }
    
    logger(LOG_ERROR, "Loop wrong, never be here!" ERROR_CODE_FMT, ERROR_CODE_STR);
}

#endif