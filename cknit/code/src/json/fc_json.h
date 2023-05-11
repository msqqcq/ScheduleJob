/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef FASTCJSON_FC_JSON_H
#define FASTCJSON_FC_JSON_H

#include <config.h>
#include <fc_list.h>

/**
 * @brief NOTICE
 * CJSON data structure use FC_LIST
 * the type of Array or Object use the FC_LIST's key to identify
 * CJSON_INT means to use `long long` type
 * CJSON_DOUBLE's real type is `double`
 */

enum CJSON_VAL_TYPE{
CJSON_INT = 1, CJSON_DOUBLE = 2, CJSON_STRING = 4, CJSON_ARRAY = 8,
CJSON_OBJECT = 16, CJSON_NULL = 32, CJSON_BOOL = 64 };
typedef struct _FAST_CJSON_NODE
{
    char val_type;
    char  is_null;
    char  is_bool;
    char     *key;
    void     *val;
} CJSON_NODE;

#define CJSON_TYPE(c) (c).val_type
#define CJSON_NULL(c) (c).is_null
#define CJSON_BOOL(c) (c).is_bool
#define CJSON_KEY(c)  (c).key
#define CJSON_VAL(c)  (c).val
#define CJSON_TYPE_P(c) CJSON_TYPE(*(c))
#define CJSON_NULL_P(c) CJSON_NULL(*(c))
#define CJSON_BOOL_P(c) CJSON_BOOL(*(c))
#define CJSON_KEY_P(c)  CJSON_KEY(*(c))
#define CJSON_VAL_P(c)  CJSON_VAL(*(c))

#define FAST_CJSON FCL_LIST
#define FAST_CJSON_TYPE FCL_LIST_KEY
#define FAST_CJSON_TYPE_P FCL_LIST_KEY_P
#define TAIL_FAST_CJSON(cjson) CJSON_VAL_P((CJSON_NODE *)FCL_NODE_DATA_P(FCL_LIST_TAIL_P(cjson)))

CJSON_NODE *new_fast_cjson_value();
char *new_cjson_string(char *s, unsigned long l);
long long *new_cjson_int(long long l);
double *new_cjson_double(double l);
int trash_cjson_data(void *v);

FAST_CJSON *new_fast_cjson(char type);
int cjson_object_add_int_tag(FAST_CJSON *fc, char *key, long key_len, long long v, char vtyp);
int cjson_object_add_int( FAST_CJSON *fc,    char *key, long key_len, long long v);
int cjson_object_add_double( FAST_CJSON *fc, char *key, long key_len, double v);
int cjson_object_add_string( FAST_CJSON *fc, char *key, long key_len, char  *v, long v_len);
int cjson_object_add_cjson(  FAST_CJSON *fc, char *key, long key_len, void  *p, char type);
int cjson_array_add_string( FAST_CJSON *fc, char *v, long v_len);
#define  cjson_array_add_int( fc, v )      cjson_object_add_int( fc, NULL, 0, v )
#define  cjson_array_add_int_tag( fc, v, vtyp )  cjson_object_add_int_tag( fc, NULL, 0, v, vtyp )
#define  cjson_array_add_double( fc, v )   cjson_object_add_double( fc, NULL, 0, v )
#define  cjson_array_add_cjson( fc, v, t)  cjson_object_add_cjson( fc, NULL, 0, v, t )

int         fast_cjson_destroy(FAST_CJSON *fc);
char       *encode_cjson(FAST_CJSON *fc, int );
FAST_CJSON *decode_cjson(char *json_string);
void       *cjson_find_key(FAST_CJSON *fc, char *key, unsigned long key_len);
void       *cjson_find_index(FAST_CJSON *fc, unsigned long index);
char       *cjson_get_error();

#endif /* FASTCJSON_FC_JSON_H */
