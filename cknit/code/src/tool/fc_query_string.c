/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <fc_query_string.h>
#include <config.h>

static
CQUERY_NODE *
new_cquery_string()
{
    struct _FC_QUERY_STRING *ptr = malloc( sizeof(struct _FC_QUERY_STRING) );
    if (!ptr) {
        return NULL;
    }
    e_memzero(ptr, sizeof(struct _FC_QUERY_STRING));
    return ptr;
}

static
int
trash_cquery_string(void *data)
{
    if (!data) {
        return FALSE;
    }
    struct _FC_QUERY_STRING *v = data;
    e_memfree(v->key);
    e_memfree(v->val);
    e_memfree(v);
    return TRUE;
}

CQUERY *
new_cquery_from_string(char *str)
{
/**
 * @brief NOTICE
 * a=b&c=d
 * 0123456
 * (s)(p)(e)
 */
    CQUERY                *r;
    CQUERY_NODE           *n;
    unsigned long i, p, s, e;
    
    s = 0;
    i = 0;
    e = 0;
    p = 0;
    E_SKIP_CHAR(str, '?', i);
    s = i;
    
    r = new_fcl_list();
    
    for ( ; str[i] != '\0'; i++ ) {
        
        if ( str[i] == '=' ) {
            p = i;
        }
        elif ( str[i] == '&' ) {
            e = i;
        }
        
        if ( str[i+1] == '\0' ) {
            e = i + 1;
        }
        
        if ( p && e ) {
            n = new_cquery_string();
            
            n->key = e_substr(str + s, p - s);
            n->key_len = p - s;
            if ( e - p > 1 ){
                n->val     = e_substr( str + p + 1, e - p - 1 );
                n->val_len = e - p - 1;
            }
            fcl_list_push(r, n, trash_cquery_string);
            s = e + 1;
            p = e = 0;
        }
    }
    
    return r;
}

/**
 * @brief NOTICE
 * Below are the macro for parsing the QueryString for the string:
 * <== ?name=Josin&age=26 or name=Josin&age=26 ==>
 */
CQUERY_FIELD_FUNC(task)
    CQUERY_FIELD_CMP(from, 4, if)
CQUERY_FIELD_FUNC_END();