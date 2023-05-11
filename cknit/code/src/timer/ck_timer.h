/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */


#ifndef CKNIT_CK_TIMER_H
#define CKNIT_CK_TIMER_H

#include <time.h>
#include <sys/time.h>
#include <sys/select.h>

#define CK_TIMER(t) select(0, NULL, NULL, NULL, (t))

#endif /* CKNIT_CK_TIMER_H */
