/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <log.h>
#include <estring.h>
#include <http_request.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

CSTRING *
new_cstring()
{
    CSTRING *ptr = malloc( sizeof(CSTRING) );
    if (!ptr) {
        return NULL;
    }
    e_memzero(ptr, sizeof(CSTRING));
    return ptr;
}

void
new_cstring_free(CSTRING *c)
{
    if(!c) return;
    e_memfree(c->s);
    e_memfree(c);
}

int
new_cstring_add_long_long(CSTRING *c, long long i)
{
    char            *t;
    char            buff[20];
    unsigned long   sl;
    if(!c) {
        return FALSE;
    }
    e_memzero(buff, sizeof(buff));
    sprintf(buff, "%lld", i);
    
    if (c->t - c->l > sizeof(buff)) {
        e_copymem(c->s + c->l, buff, sizeof(char) * sizeof(buff));
        c->l += strlen(buff);
    } else {
        if ( BUFFER_SIZE > sizeof(buff) )
        {
            sl = c->l + BUFFER_SIZE;
        }
        t = malloc( sizeof(char) * sl );
        e_memzero(t, sizeof(char) * sl );
        e_copymem(t, c->s, sizeof(char) * (c->l));
        e_copymem(t + c->l, buff, sizeof(buff) * sizeof(char));
        e_memfree(c->s);
        c->s = t;
        c->t = sl;
        c->l += sizeof(buff);
    }
    return TRUE;
}

int
new_cstring_add_double(CSTRING *c, double d)
{
    char            *t;
    char            buff[100];
    unsigned long   sl;
    if(!c) {
        return FALSE;
    }
    e_memzero(buff, sizeof(buff));
    sprintf(buff, "%f", d);
    
    if (c->t - c->l > sizeof(buff)) {
        e_copymem(c->s + c->l, buff, sizeof(char) * sizeof(buff));
        c->l += strlen(buff);
    } else {
        if ( BUFFER_SIZE > sizeof(buff) )
        {
            sl = c->l + BUFFER_SIZE;
        }
        t = malloc( sizeof(char) * sl );
        e_memzero(t, sizeof(char) * sl );
        e_copymem(t, c->s, sizeof(char) * (c->l));
        e_copymem(t + c->l, buff, sizeof(char) * sizeof(buff));
        e_memfree(c->s);
        c->s = t;
        c->l+= sizeof(buff);
        c->t = sl;
    }
    return TRUE;
}

int
new_cstring_add_string(CSTRING *c, char *s, long l)
{
    char            *t;
    unsigned long   sl;
    if(!c) {
        return FALSE;
    }
    
    if (c->t - c->l > l) {
        e_copymem(c->s + c->l, s, sizeof(char) * l);
        c->l+= l;
    } else {
        if ( BUFFER_SIZE > l )
        {
            sl = c->l + BUFFER_SIZE;
        }
        else
        {
            sl = c->l + l + 1;
        }
        t = malloc( sizeof(char) * sl );
        e_memzero(t, sizeof(char) * sl );
        e_copymem(t, c->s, sizeof(char) * (c->l));
        e_copymem(t + c->l, s, sizeof(char) * l);
        e_memfree(c->s);
        c->s = t;
        c->l+= l;
        c->t+= sl;
    }
    return TRUE;
}

int
new_cstring_add_char(CSTRING *c, char v)
{
    char            *t;
    unsigned long   sl;
    if( !c ) {
        return FALSE;
    }
    
    if (c->t - c->l > sizeof(char) ) {
        e_copymem(c->s + c->l, &v, sizeof(char) * sizeof(v));
        c->l += sizeof(v);
    } else {
        if ( BUFFER_SIZE > sizeof(v) )
        {
            sl = c->l + BUFFER_SIZE;
        }
        t = malloc( sizeof(char) * sl );
        e_memzero(t, sizeof(char) * sl );
        e_copymem(t, c->s, sizeof(char) * (c->l));
        e_copymem(t + c->l, &v, sizeof(char));
        e_memfree(c->s);
        c->s = t;
        c->t+= sl;
        c->l+= sizeof(v);
    }
    return TRUE;
}

int
new_cstring_0(CSTRING *c)
{
    char    *tv;
    if ( !c || !c->s ) {
        return FALSE;
    }
    
    if ( c->l + 1 > c->t) {
        tv = malloc( sizeof(char) * ( c->l + 1 ));
        e_memzero(tv, sizeof(char) * (c->l + 1));
        e_copymem(tv, c->s, c->l * sizeof(char));
        e_copymem(tv + c->l, "\0", sizeof(char));
    }
    else
    {
        e_copymem(c->s + c->l, "\0", sizeof(char));
    }
    return TRUE;
}

inline
char *e_substr(char *src, unsigned long len)
{
/**
 * @brief This function will automatically add '\0' at the end of the string
 */
    if (!src) return NULL;
    char *res;
    res = malloc( sizeof(char) * (len + 1) );
    if ( !res ) return NULL;
    e_memzero(res, sizeof(char) * (len + 1));
    e_copymem(res, src, sizeof(char) * len);
    return res;
}

long e_concat_str(char **dest, char *src, unsigned long len, int _reset)
{
/**
 * @brief This function's first parameter must be NULL when first time invoking
 * After then e_concat_str will automatically append after the previous invoking result.
 */
    char       *t;
    static long al = 0;
    if(_reset)  al = 0;
    
    t = realloc(*dest, sizeof(char) * (al + len));
    if(!t)
    {
        logger(LOG_ERROR, "Memory alloc failed: %s", "e_concat_str");
        return al;
    }
    e_copymem(t + al, src, sizeof(char) * len);
   *dest = t;
    al  += len;
    return al;
}

void e_parse_range(const char *str, long *from, long *to)
{
/**
 * @brief Introduction
 * Get range start and end from the given string
 */
    if ( e_str6cmp(str, "bytes=") )
    {
        *from = strtol(str + 6, NULL, 10);
    }
    for ( ;; )
    {
        if ( *str++ =='-' )
            break;
    }
    *to = strtol(str, NULL, 10);
}

char *e_size_data_from_file(char *file, long from, long to)
{
/**
 * @brief Introduction
 * Read size data from the given file
 * Function was binary safe
 */
    FILE    *fp;
    char    *r;
    long     l;
    size_t   u,   a,  e;
    
    a = 0;
    l = to - from + 1;
    
    fp = fopen(file, "r");
    if ( fp == NULL ) { fclose(fp); return NULL; }
    
    r = malloc( sizeof(char) * l );
    if ( r == NULL )
    {
        fclose(fp);
        return NULL;
    }
    
    for ( ;; )
    {
        if ( l == 0 ) break;
        if ( l >= BUFFER_SIZE )
        {
            e = BUFFER_SIZE;
        }
        else
        {
            e = ( size_t )l;
        }
        fseek(fp, from + a, SEEK_SET);
        u = fread(r + a, sizeof(char), sizeof(char) * e, fp);
        if ( u > 0 )
        {
            a += u;
            l -= u;
        }
    }
    fclose(fp);
    return r;
}

char *e_data_from_file(char *file, long *size)
{
/**
 * @brief Introduction
 * Read all file data from the given file name and set the `size` parameter to the file size
 * Function was binary safe
 */
    FILE    *fp;
    struct stat  fs;
    size_t   bl, al, ul;
    char    *r,  bf[BUFFER_SIZE];
    
    ul = al = 0;
    
    fp = fopen(file, "r");
    if ( fp == NULL ) { return NULL; }
    fstat(fileno(fp), &fs);
    if (fs.st_size == 0) { fclose(fp); return NULL; }
    if ( size )
        *size = fs.st_size;
    r = malloc( sizeof(char) * (fs.st_size + 1) );
    
    while (fs.st_size)
    {
        e_memzero(bf, sizeof(bf));
        bl = fread(bf, sizeof(char), sizeof(bf), fp);
        if ( bl > 0 )
        {
            fs.st_size -= bl;
            al         += bl;
            fseek(fp, al, SEEK_SET);
            e_copymem(r + ul, bf, sizeof(char) * bl);
            ul += bl;
        }
    }
    
    r[al] = '\0';
    fclose(fp);
    return r;
}

void  e_write_to_file(char *file, char *fmt, ...)
{
    FILE    *fp;
    va_list arg;
    
    fp = fopen(file, "w");
    
    if ( fp == NULL ) {
        printf("%s\n", strerror(errno));
        return ;
    }
    
    va_start(arg, fmt);
    vfprintf(fp, fmt, arg);
    va_end(arg);
    fclose(fp);
}

/**
 * @brief Introduction
 * Following function was used for parsing the period
 * match the second & minute & hour & day & month & week
 * if match success, return 1 other than 0
 * period:
 *  [ * * * 1-3 * * ]
 *  will be parsed to the following vars:
 *  second: Every second
 *  minute: Every minute
 *  hour  : Every hour
 *  day   : Day 1 to Day 3
 *  month : Every month
 *  week  : Every week
 *
 *  Support the below special char:
 *  * : Every
 *  - : One to another
 *  , : One and another
 *  / : Every another
 */
int e_parse_second(struct tm *time_v, char *period, int *n)
{
    int  i, p, min, max, pr, k, is_co, is_sl;
    
    i = *n;
    
    E_SKIP_SPACE(period, i);
    if ( period[i] == '\0' ) return FALSE;
    *n = i;
    
    if (period[*n] == '*' && period[*n + 1] != '/')
    {
        return 1;
    }
    
    is_co = 0;
    is_sl = 0;
    p     = *n;
/**
* @brief Introduction
* Check when before next ',' there is not '-' occur
* before the ' ' char, there is
*/
    E_PARSE_CRON(tm_sec);
    
    
    return 0;
}

int e_parse_minute(struct tm *time_v, char *period, int *n)
{
    int  i, p, min, max, pr, k, is_co, is_sl;
    
    i = *n;
    
    E_SKIP_SPACE(period, i);
    if ( period[i] == '\0' ) return FALSE;
    *n = i;
    
    if (period[*n] == '*' && period[*n + 1] != '/')
    {
        return 1;
    }
    
    is_co = 0;
    is_sl = 0;
    p     = *n;
/**
* @brief Introduction
* Check when before next ',' there is not '-' occur
* before the ' ' char, there is
*/
    E_PARSE_CRON(tm_min);
    
    return 0;
}

int e_parse_hour(struct tm *time_v, char *period, int *n)
{
    int  i, p, min, max, pr, k, is_co, is_sl;
    
    i = *n;
    
    E_SKIP_SPACE(period, i);
    if ( period[i] == '\0' ) return FALSE;
    *n = i;
    
    if (period[*n] == '*' && period[*n + 1] != '/')
    {
        return 1;
    }
    
    is_co = 0;
    is_sl = 0;
    p     = *n;
/**
* @brief Introduction
* Check when before next ',' there is not '-' occur
* before the ' ' char, there is
*/
    E_PARSE_CRON(tm_hour);
    
    return 0;
}

int e_parse_day(struct tm *time_v, char *period, int *n)
{
    int  i, p, min, max, pr, k, is_co, is_sl;
    
    i = *n;
    
    E_SKIP_SPACE(period, i);
    if ( period[i] == '\0' ) return FALSE;
    *n = i;
    
    if (period[*n] == '*' && period[*n + 1] != '/')
    {
        return 1;
    }
    
    is_co = 0;
    is_sl = 0;
    p     = *n;
/**
* @brief Introduction
* Check when before next ',' there is not '-' occur
* before the ' ' char, there is
*/
    E_PARSE_CRON(tm_mday);
    
    return 0;
}

int e_parse_month(struct tm *time_v, char *period, int *n)
{
    int  i, p, min, max, pr, k, is_co, is_sl;
    
    i = *n;
    
    E_SKIP_SPACE(period, i);
    if ( period[i] == '\0' ) return FALSE;
    *n = i;
    
    if (period[*n] == '*' && period[*n + 1] != '/')
    {
        return 1;
    }
    
    is_co = 0;
    is_sl = 0;
    p     = *n;
/**
* @brief Introduction
* Check when before next ',' there is not '-' occur
* before the ' ' char, there is
*/
    E_PARSE_CRON(tm_mon);
    
    return 0;
}

int e_parse_week(struct tm *time_v,char *period, int *n)
{
    int  i, p, min, max, pr, k, is_co, is_sl;
    
    i = *n;
    
    E_SKIP_SPACE(period, i);
    if ( period[i] == '\0' ) return FALSE;
    *n = i;
    
    if (period[*n] == '*' && period[*n + 1] != '/')
    {
        return 1;
    }
    
    is_co = 0;
    is_sl = 0;
    p     = *n;
/**
* @brief Introduction
* Check when before next ',' there is not '-' occur
* before the ' ' char, there is
*/
    E_PARSE_CRON(tm_wday);
    
    return 0;
}

int e_match_time(char *period)
{
    int         i, r;
    time_t      t;
    struct tm  *time_v;
    
    i       = 0;
    t       = time(NULL);
    time_v  = localtime(&t);
    
    r = e_parse_second(time_v, period, &i);
    if ( !r ) return 0;
    
    E_TO_CHAR(period, ' ', i);
    r = e_parse_minute(time_v, period, &i);
    if ( !r ) return 0;
    
    E_TO_CHAR(period, ' ', i);
    r = e_parse_hour(time_v, period, &i);
    if ( !r ) return 0;
    
    E_TO_CHAR(period, ' ', i);
    r = e_parse_day(time_v, period, &i);
    if ( !r ) return 0;
    
    E_TO_CHAR(period, ' ', i);
    r = e_parse_month(time_v, period, &i);
    if ( !r ) return 0;
    
    E_TO_CHAR(period, ' ', i);
    r = e_parse_week(time_v, period, &i);
    if ( !r ) return 0;
    
    return 1;
}

#pragma clang diagnostic pop