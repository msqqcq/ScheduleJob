/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <list.h>
#include <errno.h>

inline
QUEUE_LIST_NODE *INIT_NODE()
{
    QUEUE_LIST_NODE *v = malloc( sizeof(QUEUE_LIST_NODE) );
    if (!v)
    {
        fprintf(stderr, "Malloc memory error: %d, msg: %s\n", errno, strerror(errno));
        exit(0);
    }
    memset(v, 0, sizeof(QUEUE_LIST_NODE));
    return v;
}

inline
QUEUE_LIST *INIT_LIST()
{
    QUEUE_LIST *v = malloc( sizeof(QUEUE_LIST) );
    if (!v)
    {
        fprintf(stderr, "Malloc memory error: %d, msg: %s\n", errno, strerror(errno));
        exit(0);
    }
    memset(v, 0, sizeof(QUEUE_LIST));
    return v;
}

int list_append(QUEUE_LIST *l, void *v, list_node_data_destruct_func f)
{
    if (!l) return 0;
    
    QUEUE_LIST_NODE *head, *data;
    head = QUEUE_LIST_HEAD_P(l);
    data = INIT_NODE();
    QUEUE_LIST_NODE_DATA_P(data) = v;
    if (f) QUEUE_LIST_NODE_FUNC_P(data) = f;
    
    if (!head)
    {
        QUEUE_LIST_TAIL_P(l) = QUEUE_LIST_HEAD_P(l) = data;
    }
    else
    {
        QUEUE_LIST_NODE_PREV_P(head) = data;
        QUEUE_LIST_NODE_NEXT_P(data) = head;
        QUEUE_LIST_HEAD_P(l) = data;
    }
    
    QUEUE_LIST_NUM_P(l)++;
    return 1;
}

int list_push(QUEUE_LIST *l, void *v, list_node_data_destruct_func f)
{
    if (!l) return 0;
    QUEUE_LIST_NODE *tail, *data;
    tail = QUEUE_LIST_TAIL_P(l);
    data = INIT_NODE();
    
    QUEUE_LIST_NODE_DATA_P(data) = v;
    if (f) QUEUE_LIST_NODE_FUNC_P(data) = f;
    
    if (!tail)
    {
        QUEUE_LIST_TAIL_P(l) = QUEUE_LIST_HEAD_P(l) = data;
    }
    else
    {
        QUEUE_LIST_NODE_NEXT_P(tail) = data;
        QUEUE_LIST_NODE_PREV_P(data) = tail;
        QUEUE_LIST_TAIL_P(l) = data;
    }
    
    QUEUE_LIST_NUM_P(l)++;
    return 1;
}

void list_destroy(QUEUE_LIST *l)
{
    if (!l) return ;
    QUEUE_LIST_NODE *t, *v, *head = QUEUE_LIST_HEAD_P(l);
    if (head)
    {
        t = head;
        while (t)
        {
            v = QUEUE_LIST_NODE_NEXT_P(t);
            if (QUEUE_LIST_NODE_FUNC_P(t))
            {
                QUEUE_LIST_NODE_FUNC_P(t)(QUEUE_LIST_NODE_DATA_P(t));
            }
            e_memfree(t);
            t = v;
        }
    }
    e_memfree(l);
}