/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef CKNIT_HELP_H
#define CKNIT_HELP_H

#include <list.h>
#include <fc_json.h>

#define CJSON_FIELD_DEF(n) typedef struct _FIELD_KEY_##n { CJSON_NODE
#define CJSON_FIELD_DEF_END(n) ; int oth;}CJSON_##n
#define CJSON_FIELD_FUNC_DEF(n)\
CJSON_##n *NEW_CJSON_##n();\
void TRASH_CJSON_##n(CJSON_##n *task);\
CJSON_##n *NEW_CJSON_##n##_FROM_CJSON(FAST_CJSON *cjson)
#define CJSON_FIELD_FUNC(n)\
inline CJSON_##n *NEW_CJSON_##n()\
{\
    void *ptr = malloc( sizeof(CJSON_##n) );\
    if ( !ptr ) {\
        return NULL;\
    }\
    e_memzero(ptr, sizeof(CJSON_##n));\
    return ptr;\
}\
void TRASH_CJSON_##n(CJSON_##n *task)\
{\
    if (!task) {\
        return ;\
    }\
    e_memfree(task);\
}\
CJSON_##n *NEW_CJSON_##n##_FROM_CJSON(FAST_CJSON *cjson)\
{\
    CJSON_##n  *task;\
    FCL_NODE    *node;\
    CJSON_NODE *cnode;\
    if ( !cjson || !(FAST_CJSON_TYPE_P(cjson) & CJSON_OBJECT) ) {\
        return NULL;\
    }\
    task = NEW_CJSON_##n();\
    task->oth = 0;\
    FCL_LIST_FOREACH_HEAD(cjson, node) {\
        cnode = FCL_NODE_DATA_P(node);\
        if (!cnode) {\
            TRASH_CJSON_##n(task);\
            return NULL;\
        }
#define CJSON_FIELD_FUNC_END() \
        else {\
            task->oth = 1;\
        }\
    } FCL_LIST_FOREACH_END();\
    return task;\
}
#define CJSON_FIELD_CMP(name, l, e)\
e ( e_str##l##cmp(CJSON_KEY_P(cnode), #name) )\
{\
    task->name = cnode;\
}

/**
 * @brief NOTICE
 * You must define the `_v` to (void *) type to contain all types of data
 * such as:
 *      `void *ptr;`
 * if you want to use the CJSON_NODE, you can use the inner var `cnode`
 * or if you want to use the FCL_NODE, you can use the `node` var
 * CJSON_ARRAY_FOREACH(c, ptr) {
 *    // Some thing you want to do
 *    // ...
 * } CJSON_ARRAY_FOREACH_END();
 */
#define CJSON_ARRAY_FOREACH(_c, _v) do { \
FCL_NODE    *node; \
CJSON_NODE  *cnode;\
FCL_LIST_FOREACH_HEAD(_c, node) { \
    cnode = FCL_NODE_DATA_P(node);\
    if ( !cnode ) {\
        return 0;\
    }\
    _v  = CJSON_VAL_P(cnode);
#define CJSON_ARRAY_FOREACH_END() \
} FCL_LIST_FOREACH_END(); }while(0)

void  *list_find_id(FAST_CJSON *v, long i);
int    list_delete_id(FAST_CJSON *v, long i);

#endif /* CKNIT_HELP_H */