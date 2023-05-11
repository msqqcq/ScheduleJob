/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <help.h>
#include <ck_task.h>

void *list_find_id(FAST_CJSON *v, long i)
{
    if (!v)
        return NULL;
    
    long long   idex;
    FCL_NODE   *node;
    FAST_CJSON *tjson;
    CJSON_NODE *cnode;
    
    FCL_LIST_FOREACH_HEAD(v, node) {
        cnode = FCL_NODE_DATA_P(node);
        if ( CJSON_TYPE_P(cnode) != CJSON_OBJECT )
        {
            continue;
        }
        tjson = CJSON_VAL_P(cnode);
        idex  = *(long long *)cjson_find_key(tjson, E_STRL("id"));
        if (idex == i) { return tjson; };
    } FCL_LIST_FOREACH_END();
    
    return NULL;
}

void trash_fcl_node_cjson_node(FCL_NODE *node)
{
/**
 * @brief NOTICE
 * This function only care about the top level free, if contains the array or object
 * This function may be free memory not fully.
 */
    FAST_CJSON *cjson;
    FCL_NODE   *tnode;
    CJSON_NODE *cnode, *tmp;
    if ( !node ) return;
    cnode = FCL_NODE_DATA_P(node);
    cjson = CJSON_VAL_P(cnode);
    
    FCL_LIST_FOREACH_HEAD(cjson, tnode) {
        tmp = FCL_NODE_DATA_P(tnode);
        e_memfree(CJSON_KEY_P(tmp));
        e_memfree(CJSON_VAL_P(tmp));
        e_memfree(tmp);
        e_memfree(tnode);
    } FCL_LIST_FOREACH_END();
    
    e_memfree(CJSON_KEY_P(cnode));
    e_memfree(cjson);
    e_memfree(cnode);
    e_memfree(node);
}

int list_delete_id(FAST_CJSON *v, long i)
{
    if (!v) return 0;
    
    while(true)
    {
        if ( !pthread_mutex_trylock(&status) ) { continue; }
        else break;
    }
    
    long long   idex;
    FCL_NODE   *node;
    FAST_CJSON *tjson;
    CJSON_NODE *cnode;
    
    FCL_LIST_FOREACH_HEAD(v, node) {
        cnode = FCL_NODE_DATA_P(node);
        if ( CJSON_TYPE_P(cnode) != CJSON_OBJECT ){
            continue;
        }
        tjson =  CJSON_VAL_P(cnode);
        idex  = *(long long *)cjson_find_key(tjson, E_STRL("id"));
        if (idex == i) {
            if ( FCL_NODE_NEXT_P(node) && FCL_NODE_PREV_P(node) )
            {
/**
 * @brief NOTICE
 * Have prev. & next node
 */
                FCL_NODE_NEXT_P(FCL_NODE_PREV_P(node)) = FCL_NODE_NEXT_P(node);
                FCL_NODE_PREV_P(FCL_NODE_NEXT_P(node)) = FCL_NODE_PREV_P(node);
                trash_fcl_node_cjson_node(node);
                return 1;
            }
            elif ( FCL_NODE_NEXT_P(node) && !FCL_NODE_PREV_P(node) )
            {
/**
 * @brief NOTICE
 * Have next but not prev.
 * 1. Set FCL_LIST's head node to current node's next node
 * 2. Set current's next node's prev. node to NULL
 * 3. trash current node.
 */
                FCL_LIST_HEAD_P(v) = FCL_NODE_NEXT_P(node);
                FCL_NODE_PREV_P(FCL_NODE_NEXT_P(node)) = NULL;
                trash_fcl_node_cjson_node(node);
                return 1;
            }
            elif ( !FCL_NODE_NEXT_P(node) && FCL_NODE_PREV_P(node) )
            {
/**
 * @brief NOTICE
 * Have Prev. but not next.
 * 1. Set FCL_LIST's tail node to current's node's prev. node
 * 2. Set current's prev. node's next node to NULL
 * 3. trash current node.
 */
                FCL_LIST_TAIL_P(v) = FCL_NODE_PREV_P(node);
                FCL_NODE_NEXT_P(FCL_NODE_PREV_P(node)) = NULL;
                trash_fcl_node_cjson_node(node);
                return 1;
            }
            elif ( !FCL_NODE_NEXT_P(node) && !FCL_NODE_PREV_P(node) )
            {
/**
 * @brief NOTICE
 *  Next & Prev. node is empty
 */
                FCL_LIST_TAIL_P(v) = NULL;
                FCL_LIST_HEAD_P(v) = NULL;
                trash_fcl_node_cjson_node(node);
                return 1;
            }
        };
    } FCL_LIST_FOREACH_END();

    pthread_mutex_unlock(&status);
    return -1;
}