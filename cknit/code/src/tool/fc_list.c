/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <estring.h>
#include <fc_list.h>
#include <fc_tool.h>

inline
FCL_NODE *new_fcl_node()
{
    FCL_NODE *ptr = malloc( sizeof(FCL_NODE) );
    if ( !ptr ) {
        FC_ERROR_EXIT("Malloc memory in new_fcl_node error\n");
    }
    e_memzero(ptr, sizeof(FCL_NODE));
    return ptr;
}

inline
FCL_LIST *new_fcl_list()
{
    FCL_LIST *ptr = malloc( sizeof(FCL_LIST) );
    if ( !ptr ) {
        FC_ERROR_EXIT("Malloc memory in new_fcl_list error\n");
    }
    e_memzero(ptr, sizeof(FCL_LIST));
    return ptr;
}

int fcl_list_push(FCL_LIST    *fcl_list, void *val, trash_func func)
{
    if (!fcl_list) { return FALSE; }
    
    FCL_NODE *node;
    
    node = new_fcl_node();
    FCL_NODE_DATA_P(node) = val;
    FCL_NODE_FUNC_P(node) = func;
    
    if ( FCL_LIST_TAIL_P(fcl_list) )
    {
/**
 * @brief NOTICE
 * Current List has tail node, only thing need to do
 * is to set the current tail node's next node to current
 * and set the given node to the list's tail node.
 */
        FCL_NODE_NEXT_P(FCL_LIST_TAIL_P(fcl_list)) = node;
        FCL_NODE_PREV_P(node) = FCL_LIST_TAIL_P(fcl_list);
        FCL_LIST_TAIL_P(fcl_list) = node;
    }
    else
    {
/**
 * @brief NOTICE
 * Current List hasn't the tail node & head node
 */
        FCL_LIST_TAIL_P(fcl_list) = node;
        FCL_LIST_HEAD_P(fcl_list) = node;
    }
    
    FCL_LIST_NUM_P(fcl_list)++;
    return TRUE;
}

int fcl_list_append(FCL_LIST  *fcl_list, void *val, trash_func func)
{
    if (!fcl_list) { return FALSE; }
    
    FCL_NODE *node;
    
    node = new_fcl_node();
    FCL_NODE_DATA_P(node) = val;
    FCL_NODE_FUNC_P(node) = func;
    
    if ( FCL_LIST_HEAD_P(fcl_list) )
    {
/**
 * @brief NOTICE
 * Current List owns the head node
 */
        FCL_NODE_PREV_P(FCL_LIST_HEAD_P(fcl_list)) = node;
        FCL_NODE_NEXT_P(node) = FCL_LIST_HEAD_P(fcl_list);
        FCL_LIST_HEAD_P(fcl_list) = node;
    }
    else
    {
/**
 * @brief NOTICE
 * Current List hasn't the head node
 */
        FCL_LIST_HEAD_P(fcl_list) = node;
        FCL_LIST_TAIL_P(fcl_list) = node;
    }
    
    FCL_LIST_NUM_P(fcl_list)++;
    return TRUE;
}

inline
int fcl_node_destroy(FCL_NODE *fcl_node)
{
    if ( !fcl_node ) return FALSE;
    e_memfree(FCL_NODE_DATA_P(fcl_node));
    e_memfree(fcl_node);
    return TRUE;
}

int fcl_list_destroy(FCL_LIST *fcl_list)
{
    if ( !fcl_list ) return FALSE;
    
    FCL_NODE *node, *temp;
    
    temp = node = FCL_LIST_HEAD_P(fcl_list);
    
    while (temp)
    {
        node = temp;
        temp = FCL_NODE_NEXT_P(node);
        if ( FCL_NODE_FUNC_P(node) )
        {
            FCL_NODE_FUNC_P(node)(FCL_NODE_DATA_P(node));
        }
        e_memfree(node);
    }
    
    e_memfree(fcl_list);
    return TRUE;
}
