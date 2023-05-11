/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef FASTCJSON_FC_LIST_H
#define FASTCJSON_FC_LIST_H

typedef int (*trash_func)(void *v);

typedef struct _FC_LIST_NODE
{
    void                 *data;
    struct _FC_LIST_NODE *next;
    struct _FC_LIST_NODE *prev;
    trash_func            func;
} FCL_NODE;

#define FCL_NODE_DATA(n)   (n).data
#define FCL_NODE_DATA_P(n) FCL_NODE_DATA(*(n))
#define FCL_NODE_NEXT(n)   (n).next
#define FCL_NODE_NEXT_P(n) FCL_NODE_NEXT(*(n))
#define FCL_NODE_PREV(n)   (n).prev
#define FCL_NODE_PREV_P(n) FCL_NODE_PREV(*(n))
#define FCL_NODE_FUNC(n)   (n).func
#define FCL_NODE_FUNC_P(n) FCL_NODE_FUNC(*(n))

typedef struct _FC_LIST
{
    FCL_NODE     *head,
                 *tail;
    char          key;  /* Can be used for other case */
    unsigned long num;
} FCL_LIST;

#define FCL_LIST_HEAD(l)   (l).head
#define FCL_LIST_HEAD_P(l) FCL_LIST_HEAD(*(l))
#define FCL_LIST_KEY(l)   (l).key
#define FCL_LIST_KEY_P(l) FCL_LIST_KEY(*(l))
#define FCL_LIST_TAIL(l)   (l).tail
#define FCL_LIST_TAIL_P(l) FCL_LIST_TAIL(*(l))
#define FCL_LIST_NUM(l)    (l).num
#define FCL_LIST_NUM_P(l)  FCL_LIST_NUM(*(l))

#define FCL_LIST_FOREACH_HEAD(l, n) do { for ( n = FCL_LIST_HEAD_P(l); n != NULL; n = FCL_NODE_NEXT_P(n)) {
#define FCL_LIST_FOREACH_TAIL(l, n) do { for ( n = FCL_LIST_TAIL_P(l); n != NULL; n = FCL_NODE_PREV_P(n)) {
#define FCL_LIST_FOREACH_END() }} while(0)

FCL_NODE *new_fcl_node();
FCL_LIST *new_fcl_list();
int       fcl_list_push(FCL_LIST    *fcl_list, void *val, trash_func );
int       fcl_list_append(FCL_LIST  *fcl_list, void *val, trash_func );
int       fcl_node_destroy(FCL_NODE *fcl_node);
int       fcl_list_destroy(FCL_LIST *fcl_list);


#endif /* FASTCJSON_FC_LIST_H */
