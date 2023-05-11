/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <fc_json.h>
#include <estring.h>
#include <fc_tool.h>

inline
CJSON_NODE *new_fast_cjson_value()
{
    CJSON_NODE *ptr = malloc(sizeof(CJSON_NODE));
    if (!ptr) {
        FC_ERROR_EXIT("Malloc memory in new_fast_cjson error.");
    }
    e_memzero(ptr, sizeof(CJSON_NODE));
    return ptr;
}

inline
int trash_cjson_data(void *v)
{
/**
 * @brief NOTICE
 * This functio used for free the used memory after using.
 */
    CJSON_NODE *data = v;
    if ( !data ) return FALSE;
    e_memfree(CJSON_KEY_P(data));
    
    if ( CJSON_TYPE_P(data) &
        ( CJSON_ARRAY | CJSON_OBJECT) )
    {
        fast_cjson_destroy(CJSON_VAL_P(data));
    }
    else
        e_memfree(CJSON_VAL_P(data));
    e_memfree(data);
    return TRUE;
}

inline
char *new_cjson_string(char *s, unsigned long l)
{
    char *str = malloc( sizeof(char) * (l + 1) );
    if (!str) { FC_ERROR_EXIT("Malloc memory in new_cjson_string failed."); }
    e_memzero(str, sizeof(char) * (l + 1));
    e_copymem(str, s, sizeof(char) * l);
    return str;
}

inline
long long *new_cjson_int(long long l)
{
    long long *ptr = malloc( sizeof(long long) );
    if (!ptr) { FC_ERROR_EXIT("Malloc memory in new_cjson_int failed."); }
    e_memzero(ptr, sizeof(long long));
    e_copymem(ptr, &l, sizeof(long long));
    return ptr;
}

inline
double *new_cjson_double(double l)
{
    double *ptr = malloc( sizeof(double) );
    if (!ptr) { FC_ERROR_EXIT("Malloc memory in new_cjson_double failed."); }
    e_memzero(ptr, sizeof(double));
    e_copymem(ptr, &l, sizeof(double));
    return ptr;
}

inline
FAST_CJSON *new_fast_cjson(char type)
{
/**
 * @brief NOTICE
 * This function was used to generate the CJSON outer memory area
 */
    if ( !(type & (CJSON_ARRAY | CJSON_OBJECT )) )
    {
        return NULL;
    }
    FAST_CJSON *ptr = new_fcl_list();
    FAST_CJSON_TYPE_P(ptr) = type;
    return ptr;
}

int cjson_object_add_int(FAST_CJSON *fc, char *key, long key_len, long long v)
{
    CJSON_NODE *node;
    
    if (!fc) return FALSE;
    
    node                   = new_fast_cjson_value();
    if ( key && FAST_CJSON_TYPE_P(fc) == CJSON_OBJECT ) {
        CJSON_KEY_P(node)  = new_cjson_string( key, ( unsigned long )key_len );
    }
    CJSON_VAL_P(node)  = new_cjson_int(v);
    CJSON_TYPE_P(node) = CJSON_INT;
    
    fcl_list_push(fc, node, trash_cjson_data);
    return TRUE;
}

int cjson_object_add_int_tag(FAST_CJSON *fc, char *key, long key_len, long long v, char vtyp)
{
    CJSON_NODE *node;
    
    if (!fc) return FALSE;
    
    node               = new_fast_cjson_value();
    if ( key && FAST_CJSON_TYPE_P(fc) == CJSON_OBJECT ) {
        CJSON_KEY_P(node)  = new_cjson_string( key, ( unsigned long )key_len );
    }
    CJSON_VAL_P(node)  = new_cjson_int(v);
    CJSON_TYPE_P(node) = vtyp;
    
    fcl_list_push(fc, node, trash_cjson_data);
    return TRUE;
}

int cjson_object_add_double(FAST_CJSON *fc, char *key, long key_len, double v)
{
    CJSON_NODE *node;
    
    if (!fc) return FALSE;
    
    node               = new_fast_cjson_value();
    if ( key && FAST_CJSON_TYPE_P(fc) == CJSON_OBJECT ) {
        CJSON_KEY_P(node)  = new_cjson_string( key, ( unsigned long )key_len );
    }
    CJSON_VAL_P(node)  = new_cjson_double(v);
    CJSON_TYPE_P(node) = CJSON_DOUBLE;
    
    fcl_list_push(fc, node, trash_cjson_data);
    return TRUE;
}

int cjson_object_add_string(FAST_CJSON *fc, char *key, long key_len, char  *v, long v_len)
{
    CJSON_NODE *node;
    
    if (!fc) return FALSE;
    
    node               = new_fast_cjson_value();
    if ( key && FAST_CJSON_TYPE_P(fc) == CJSON_OBJECT ) {
        CJSON_KEY_P( node ) = new_cjson_string( key, ( unsigned long )key_len );
    }
    CJSON_VAL_P(node)  = new_cjson_string( v, ( unsigned long )v_len );
    CJSON_TYPE_P(node) = CJSON_STRING;
    
    fcl_list_push(fc, node, trash_cjson_data);
    return TRUE;
}

inline
int cjson_array_add_string(FAST_CJSON *fc, char *v, long v_len)
{
    return cjson_object_add_string(fc, NULL, 0, v, v_len);
}

int cjson_object_add_cjson(FAST_CJSON *fc, char *key, long key_len, void  *p, char type)
{
    CJSON_NODE *node;
    
    if (!fc || !( type & (CJSON_ARRAY | CJSON_OBJECT)) ) return FALSE;
    
    node               = new_fast_cjson_value();
    if ( key && FAST_CJSON_TYPE_P(fc) == CJSON_OBJECT ) {
        CJSON_KEY_P( node ) = new_cjson_string( key, ( unsigned long )key_len );
    }
    CJSON_VAL_P(node)  = p;
    CJSON_TYPE_P(node) = type;
    
    fcl_list_push(fc, node, trash_cjson_data);
    return TRUE;
}

inline
int fast_cjson_destroy(FAST_CJSON *fc)
{
    return fcl_list_destroy(fc);
}

void *fc_realloc(void *p, size_t p_len, size_t len)
{
    void *ptr;
    ptr = malloc(len);
    if ( !ptr ) return NULL;
    e_memzero(ptr, len);
    e_copymem(ptr, p, p_len);
    e_memfree(p);
    return ptr;
}

long e_concat_str2(char **dest, char *src, unsigned long len, int _reset, long tl)
{
    /**
     * @brief This function's first parameter must be NULL when first time invoking
     * After then e_concat_str will automatically append after the previous invoking result.
     */
    char       *t;
    static long al = 0;
    if(_reset)  al = tl;
    
    t = realloc(*dest, sizeof(char) *(al + len));
    if(!t)
    {
        return al;
    }
    e_copymem(t + al, src, sizeof(char) * len);
    *dest = t;
    al  += len;
    return al;
}

char *encode_cjson(FAST_CJSON *fc, int turn)
{
    char          *res;
    char           bff[BUFFER_SIZE];
    char          *inr;
    long           len,
                   idx,
                   sen;
    FCL_NODE     *node;
    CJSON_NODE  *cnode;
    
    if ( !fc ) return NULL;
    
    res = NULL;
    len = 0;
    idx = 0;
    
    /**
     * @brief NOTICE
     * Add the header char: [ or {
     */
    switch (FAST_CJSON_TYPE_P(fc))
    {
        case CJSON_ARRAY:
            len = e_concat_str2(&res, E_STRL("["), 1, 0);
            break;
        case CJSON_OBJECT:
            len = e_concat_str2(&res, E_STRL("{"), 1, 0);
            break;
        default: {
            e_memfree(res);
            FC_ERROR_EXIT("fastcjson can't encode the data.");
        }
    }
    
    FCL_LIST_FOREACH_HEAD(fc, node) {
/**
 * @brief NOTICE
 * When the NODE's type is array or object, should to rescursive calling
 */
        cnode = FCL_NODE_DATA_P(node);
        
        if ( CJSON_KEY_P(cnode) )
        {
            sen = strlen( CJSON_KEY_P( cnode ) );
            e_concat_str2( &res, E_STRL( "\"" ), 0, 0);
            e_concat_str2( &res, CJSON_KEY_P( cnode ), ( unsigned long )sen, 0, 0);
            len = e_concat_str2( &res, E_STRL( "\":" ), 0, 0);
        }
        
        switch ( CJSON_TYPE_P(cnode) )
        {
            case CJSON_BOOL:
                if ( !turn )
                {
                    len = *( long long * )CJSON_VAL_P( cnode );
                    if ( len == 1 ) {
                        len = e_concat_str2( &res, E_STRL( "true" ), 0, 0);
                    } else {
                        len = e_concat_str2( &res, E_STRL( "false" ), 0, 0);
                    }
                    break;
                }
            case CJSON_NULL:
                if ( !turn ) {
                    len = e_concat_str2(&res, E_STRL("NULL"), 0, 0);
                    break;
                }
            case CJSON_INT:
                e_memzero(bff, sizeof(bff));
                sen = sprintf(bff, "%lld", *(long long *)CJSON_VAL_P(cnode));
                len = e_concat_str2(&res, bff, ( unsigned long )sen, 0, 0);
                break;
            case CJSON_DOUBLE:
                e_memzero(bff, sizeof(bff));
                sen = sprintf(bff, "%f", *(double *)CJSON_VAL_P(cnode));
                len = e_concat_str2(&res, bff, ( unsigned long )sen, 0, 0);
                break;
            case CJSON_STRING:
                e_concat_str2(&res, E_STRL("\""), 0, 0);
                e_concat_str2(&res, CJSON_VAL_P(cnode), strlen(CJSON_VAL_P(cnode)), 0, 0);
                len = e_concat_str2(&res, E_STRL("\""), 0, 0);
                break;
            case CJSON_ARRAY:
            case CJSON_OBJECT:
                idx++;
                inr = encode_cjson(CJSON_VAL_P(cnode), turn);
                idx--;
                len = e_concat_str2(&res, inr, strlen(inr), 1, len);
                e_memfree(inr);
                inr = NULL;
                break;
            default: {
                e_memfree(res);
                FC_ERROR_EXIT("fastcjson can't encode the data.");
            }
        }
        
        if ( FCL_NODE_NEXT_P(node) != NULL )
        {
            len = e_concat_str2(&res, E_STRL(","), 0, 0);
        }
        
    } FCL_LIST_FOREACH_END();
    
    /**
     * @brief NOTICE
     * Add the matching char : ] or }
     */
    switch (FAST_CJSON_TYPE_P(fc))
    {
        case CJSON_ARRAY:
            len = e_concat_str2(&res, E_STRL("]"), 0, 0);
            break;
        case CJSON_OBJECT:
            len = e_concat_str2(&res, E_STRL("}"), 0, 0);
            break;
        default: {
            e_memfree(res);
            FC_ERROR_EXIT("fastcjson can't encode the data.");
        }
    }
    
    if ( idx == 0 )
    {
        res = fc_realloc( res, ( size_t )len, ( size_t )( len + 1 ) );
    }

    return res;
}

void *cjson_find_key(FAST_CJSON *fc, char *key, unsigned long key_len)
{
    FCL_NODE   *node;
    CJSON_NODE *cjda;
    if ( fc->key != CJSON_OBJECT ) return NULL;
    FCL_LIST_FOREACH_TAIL(fc, node) {
        cjda = FCL_NODE_DATA_P(node);
        if ( e_memcmp(CJSON_KEY_P(cjda), key, key_len) == 0 )
        {
            return CJSON_VAL_P(cjda);
        }
    } FCL_LIST_FOREACH_END();
    return NULL;
}

void *cjson_find_index(FAST_CJSON *fc, unsigned long index)
{
    FCL_NODE   *node;
    CJSON_NODE *cjda;
    if ( fc->key != CJSON_ARRAY ) return NULL;
    
    FCL_LIST_FOREACH_HEAD(fc, node) {
        cjda = FCL_NODE_DATA_P(node);
        if (index-- == 0) return CJSON_VAL_P(cjda);
    } FCL_LIST_FOREACH_END();
    return NULL;
}

