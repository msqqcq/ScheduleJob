/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef LISTS_EV_H
#define LISTS_EV_H

#ifdef __linux__
    #include <ex_epoll.h>
#else
    #include <kqueue.h>
#endif

#endif /* LISTS_EV_H */
