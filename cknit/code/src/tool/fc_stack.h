/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef FASTCJSON_FC_STACK_H
#define FASTCJSON_FC_STACK_H

#include <config.h>

typedef int (*trash_stack_func)(void *v);

typedef struct _FC_STACK_NODE
{
    void                  *data;
    struct _FC_STACK_NODE *next;
    struct _FC_STACK_NODE *prev;
    trash_stack_func       func;
} FCN;

#define FCN_DATA(n)   (n).data
#define FCN_DATA_P(n) FCN_DATA(*(n))
#define FCN_NEXT(n)   (n).next
#define FCN_NEXT_P(n) FCN_NEXT(*(n))
#define FCN_PREV(n)   (n).prev
#define FCN_PREV_P(n) FCN_PREV(*(n))
#define FCN_FUNC(n)   (n).func
#define FCN_FUNC_P(n) FCN_FUNC(*(n))

typedef struct _FC_STACK
{
    FCN          *head;
    unsigned long num;
} FCS;
#define FCS_HEAD(s)     (s).head
#define FCS_HEAD_P(s)   FCS_HEAD(*(s))
#define FCS_NUM(s)     (s).num
#define FCS_NUM_P(s)   FCS_NUM(*(s))

#define FCS_FOREACH(s, n) do { for (n = FCS_HEAD_P(s); n != NULL; n = FCN_NEXT_P(n)) {
#define FCS_FOREACH_END() }}while(0)

FCN  *new_fcn();
FCS  *new_fcs();
int   fcs_push(FCS* s, void *p, trash_stack_func );
void *fcs_pop(FCS  *s);
int   fcs_destroy(FCS *s);
int   fcs_destroy2(FCS *s);

#endif /* FASTCJSON_FC_STACK_H */
