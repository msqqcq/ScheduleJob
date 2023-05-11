/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <fc_stack.h>
#include <estring.h>
#include <fc_tool.h>

inline
FCN *new_fcn()
{
    FCN *ptr = malloc( sizeof(FCN) );
    if ( !ptr )
    {
        FC_ERROR_EXIT("Malloc memory in new_fcn failed\n");
    }
    e_memzero(ptr, sizeof(FCN));
    return ptr;
}

inline
FCS *new_fcs()
{
    FCS *ptr = malloc( sizeof(FCS) );
    if ( !ptr )
    {
        FC_ERROR_EXIT("Malloc memory in new_fcs failed\n");
    }
    e_memzero(ptr, sizeof(FCS));
    return ptr;
}

int  fcs_push(FCS* s, void *p, trash_stack_func f)
{
    if (!s) return FALSE;
    
    FCN *node;
    node = new_fcn();
    FCN_DATA_P(node) = p;
    FCN_FUNC_P(node) = f;
    
    if ( FCS_HEAD_P(s) )
    {
/**
 * @brief NOTICE
 * Has head, so add the data to the head instead
 */
        FCN_NEXT_P(node) = FCS_HEAD_P(s);
        FCN_PREV_P(FCS_HEAD_P(s)) = node;
    }
    FCS_HEAD_P(s) = node;
    FCS_NUM_P(s)++;
    
    return TRUE;
}

inline
void *fcs_pop(FCS  *s)
{
    FCN *node; void *ptr;
    if (!s) return NULL;
    node = FCS_HEAD_P(s);
    if ( !node ) return NULL;
    FCS_HEAD_P(s) = FCN_NEXT_P(node);
    ptr = FCN_DATA_P(node);
    e_memfree(node);
    return ptr;
}

int  fcs_destroy(FCS *s)
{
    FCN *node, *temp;
    if (!s) return TRUE;
    
    temp = node = FCS_HEAD_P(s);
    while(temp)
    {
        node = temp;
        temp = FCN_NEXT_P(node);
        if ( FCN_FUNC_P(node) )
        {
            FCN_FUNC_P(node)(FCN_DATA_P(node));
        }
        e_memfree(node);
    }
    e_memfree(s);
    return TRUE;
}