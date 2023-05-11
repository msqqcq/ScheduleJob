/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef LISTS_LIST_H
#define LISTS_LIST_H

#include <config.h>

typedef void(*list_node_data_destruct_func)(void *v/* Real type decided by struct _LIST_V's data field */);

typedef struct _LIST_V
{
    void           *data;
    struct _LIST_V *next;
    struct _LIST_V *prev;
    list_node_data_destruct_func f;
} QUEUE_LIST_NODE;

#define QUEUE_LIST_NODE_DATA(node) (node).data
#define QUEUE_LIST_NODE_DATA_P(node) QUEUE_LIST_NODE_DATA(*(node))
#define QUEUE_LIST_NODE_FUNC(node) (node).f
#define QUEUE_LIST_NODE_FUNC_P(node) QUEUE_LIST_NODE_FUNC(*(node))
#define QUEUE_LIST_NODE_NEXT(node) (node).next
#define QUEUE_LIST_NODE_NEXT_P(node) QUEUE_LIST_NODE_NEXT(*(node))
#define QUEUE_LIST_NODE_PREV(node) (node).prev
#define QUEUE_LIST_NODE_PREV_P(node) QUEUE_LIST_NODE_PREV(*(node))

typedef struct _LIST
{
    QUEUE_LIST_NODE      *tail;
    QUEUE_LIST_NODE      *head;
    unsigned long         num;
} QUEUE_LIST;

#define QUEUE_LIST_TAIL(list) (list).tail
#define QUEUE_LIST_TAIL_P(list) QUEUE_LIST_TAIL(*(list))
#define QUEUE_LIST_HEAD(list) (list).head
#define QUEUE_LIST_HEAD_P(list) QUEUE_LIST_HEAD(*(list))
#define QUEUE_LIST_NUM(list) (list).num
#define QUEUE_LIST_NUM_P(list) QUEUE_LIST_NUM(*(list))

/* Some functions for create/delete the list structure */
QUEUE_LIST_NODE *INIT_NODE();
QUEUE_LIST      *INIT_LIST();
int              list_append(QUEUE_LIST *, void *, list_node_data_destruct_func);
int              list_push(QUEUE_LIST *, void *, list_node_data_destruct_func);
void             list_destroy(QUEUE_LIST *);

#define LIST_FOREACH_FROM_HEAD(list, v) do {\
QUEUE_LIST_NODE *__v, *t = QUEUE_LIST_HEAD_P(list); \
__v = t;\
for (; __v != NULL ; __v = QUEUE_LIST_NODE_NEXT_P(__v))\
{\
 v = QUEUE_LIST_NODE_DATA_P(__v);

#define LIST_FOREACH_FROM_TAIL(list, v) do {\
QUEUE_LIST_NODE *__v, *t = QUEUE_LIST_TAIL_P(list); \
__v = t;\
for (; __v != NULL ; __v = QUEUE_LIST_NODE_PREV_P(__v))\
{\
 v = QUEUE_LIST_NODE_DATA_P(__v);
 
#define LIST_FOREACH_END() }}while(0)

#endif /* LISTS_LIST_H */
