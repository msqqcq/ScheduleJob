/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 *
 * This file will parse the query string like this:
 * ?a=b&c=d
 * or not begin with `?`
 * a=b&c=d
 */

#ifndef FASTCJSON_FC_QUERY_STRING_H
#define FASTCJSON_FC_QUERY_STRING_H

#include <fc_list.h>
#include <estring.h>

#define CQUERY FCL_LIST

typedef struct _FC_QUERY_STRING
{
    char            *key,
                    *val;
    unsigned long key_len,
                  val_len;
} CQUERY_NODE;

CQUERY *new_cquery_from_string(char *str);

/**
 * @brief NOTICE
 * Some macros for easy Query string parsing and some get or set operation
 * <== CQUERY_KEY_DEF 、CQUERY_KEY_LEN_DEF 、CQUERY_FIELD_DEF 、CQUERY_FIELD_DEF_END 、CQUERY_FIELD_FUN_DEF ==>
 * used in C header file but
 * <== CQUERY_FIELD_FUNC 、CQUERY_FIELD_FUNC_END、CQUERY_FIELD_CMP ==>
 * must use in C source file
 */
#define CQUERY_KEY_DEF(...) char __VA_ARGS__
#define CQUERY_KEY_LEN_DEF(...) unsigned long __VA_ARGS__
#define CQUERY_FIELD_DEF(n) typedef struct _CQUERY_##n { CQUERY_NODE
#define CQUERY_FIELD_DEF_END(n) ;int __oth; } CQUERY_##n
#define CQUERY_FIELD_FUN_DEF(n)\
void TRASH_CQUERY_##n(CQUERY_##n *data);\
CQUERY_##n *NEW_CQUERY_##n##_FROM_CQUERY(CQUERY *query)
#define CQUERY_FIELD_FUNC(n) \
static CQUERY_##n *NEW_CQUERY_##n()\
{\
    CQUERY_##n *ptr = malloc( sizeof(CQUERY_##n) );\
    if (!ptr) {\
        return NULL;\
    }\
    e_memzero(ptr, sizeof(CQUERY_##n));\
    return ptr;\
}\
void TRASH_CQUERY_##n(CQUERY_##n *data)\
{\
    if (!data) {\
        return;\
    }\
    e_memfree(data);\
}\
CQUERY_##n *NEW_CQUERY_##n##_FROM_CQUERY(CQUERY *query)\
{\
    FCL_NODE        *node;\
    CQUERY_NODE     *cnode;\
    CQUERY_##n      *res;\
    \
    if (!query) {\
        return NULL;\
    }\
    res = NEW_CQUERY_##n();\
    if (!res) {\
        return NULL;\
    }\
    FCL_LIST_FOREACH_HEAD(query, node) {\
        cnode = FCL_NODE_DATA_P(node);\
        if ( !cnode) {\
            TRASH_CQUERY_##n(res);\
            return NULL;\
        }
#define CQUERY_FIELD_FUNC_END() \
        else {\
            res->__oth = 1;\
        }\
    } FCL_LIST_FOREACH_END();\
    return res;\
}
#define CQUERY_FIELD_CMP(name, l, e) \
e ( e_str##l##cmp(cnode->key, #name) ) { \
    res->name = cnode;\
}

/**
 * @brief NOTICE
 * Below are the QueryString parsing example which can parsing the following Query string
 * ?name=Join&age=26 or name=Josin&age=26
 */
CQUERY_FIELD_DEF(task)
    *from
CQUERY_FIELD_DEF_END(task);
CQUERY_FIELD_FUN_DEF(task);

#endif /* FASTCJSON_FC_QUERY_STRING_H */
