/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef LISTS_ESTRING_H
#define LISTS_ESTRING_H

#include <config.h>
#include <time.h>
#include <sys/time.h>

#define E_STRL(s)    (s), (sizeof(s) - 1)
#define e_tolower(c) ( (c) >= 'A' && (c) <= 'Z' ?  ( (c) | (char)0x20 )  : (c) )
#define e_toupper(c) ( ( (c) >= 'a' && (c) <= 'z' ) ? ( (c) & ~(char)0x20 ) : (c) )
#define e_isdigit(c) ( (c) - '0' >= 0 && (c) - '0' <= 9 )
#define e_memzero(s, n)  memset(s, 0, n)
#define e_copymem memcpy
#define e_memcmp memcmp
#ifndef e_memfree
#define e_memfree free
#endif

#define e_str1cmp(d, s) ((d)[0] == s[0] && (d)[1] == '\0' && (s)[1] == '\0')
#define e_str2cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == '\0' && (s)[2] == '\0')
#define e_str3cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == '\0' && (s)[3] == '\0')
#define e_str4cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == '\0' && (s)[4] == '\0')
#define e_str5cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == '\0' && (s)[5] == '\0')
#define e_str6cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == s[5] && (d)[6] == '\0' && (s)[6] == '\0')
#define e_str7cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == s[5] && (d)[6] == s[6] && (d)[7] == '\0' && (s)[7] == '\0')
#define e_str8cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == s[5] && (d)[6] == s[6] && (d)[7] == s[7] && (d)[8] == '\0' && (s)[8] == '\0')
#define e_str9cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == s[5] && (d)[6] == s[6] && (d)[7] == s[7] && (d)[8] == s[8] && (d)[9] == '\0' && (s)[9] == '\0')
#define e_str10cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == s[5] && (d)[6] == s[6] && (d)[7] == s[7] && (d)[8] == s[8] && (d)[9] == s[9] && (d)[10] == '\0' && (s)[10] == '\0')
#define e_str11cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == s[5] && (d)[6] == s[6] && (d)[7] == s[7] && (d)[8] == s[8] && (d)[9] == s[9] && (d)[10] == s[10] && (d)[11] == '\0' && (s)[11] == '\0')
#define e_str12cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == s[5] && (d)[6] == s[6] && (d)[7] == s[7] && (d)[8] == s[8] && (d)[9] == s[9] && (d)[10] == s[10] && (d)[11] == s[11] && (d)[12] == '\0' && (s)[12] == '\0')
#define e_str13cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == s[5] && (d)[6] == s[6] && (d)[7] == s[7] && (d)[8] == s[8] && (d)[9] == s[9] && (d)[10] == s[10] && (d)[11] == s[11] && (d)[12] == s[12] && (d)[13] == '\0' && (s)[13] == '\0')
#define e_str14cmp(d, s) ((d)[0] == s[0] && (d)[1] == s[1] && (d)[2] == s[2] && (d)[3] == s[3] && (d)[4] == s[4] && (d)[5] == s[5] && (d)[6] == s[6] && (d)[7] == s[7] && (d)[8] == s[8] && (d)[9] == s[9] && (d)[10] == s[10] && (d)[11] == s[11] && (d)[12] == s[12] && (d)[13] == s[13] && (d)[14] == '\0' && (s)[14] == '\0')

#define e_str1ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && (d)[1] == '\0' && (s)[1] == '\0')
#define e_str2ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && (d)[2] == '\0' && (s)[2] == '\0')
#define e_str3ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && (d)[3] == '\0' && (s)[3] == '\0')
#define e_str4ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && (d)[4] == '\0' && (s)[4] == '\0')
#define e_str5ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && (d)[5] == '\0' && (s)[5] == '\0')
#define e_str6ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && e_tolower((d)[5]) == e_tolower(s[5]) && (d)[6] == '\0' && (s)[6] == '\0')
#define e_str7ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && e_tolower((d)[5]) == e_tolower(s[5]) && e_tolower((d)[6]) == e_tolower(s[6]) && (d)[7] == '\0' && (s)[7] == '\0')
#define e_str8ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && e_tolower((d)[5]) == e_tolower(s[5]) && e_tolower((d)[6]) == e_tolower(s[6]) && e_tolower((d)[7]) == e_tolower(s[7]) && (d)[8] == '\0' && (s)[8] == '\0' )
#define e_str9ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && e_tolower((d)[5]) == e_tolower(s[5]) && e_tolower((d)[6]) == e_tolower(s[6]) && e_tolower((d)[7]) == e_tolower(s[7]) && e_tolower((d)[8]) == e_tolower(s[8]) && (d)[9] == '\0' && (s)[9] == '\0' )
#define e_str10ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && e_tolower((d)[5]) == e_tolower(s[5]) && e_tolower((d)[6]) == e_tolower(s[6]) && e_tolower((d)[7]) == e_tolower(s[7]) && e_tolower((d)[8]) == e_tolower(s[8]) && e_tolower((d)[9]) == e_tolower(s[9]) && (d)[10] == '\0' && (s)[10] == '\0' )
#define e_str11ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && e_tolower((d)[5]) == e_tolower(s[5]) && e_tolower((d)[6]) == e_tolower(s[6]) && e_tolower((d)[7]) == e_tolower(s[7]) && e_tolower((d)[8]) == e_tolower(s[8]) && e_tolower((d)[9]) == e_tolower(s[9]) && e_tolower((d)[10]) == e_tolower(s[10]) && (d)[11] == '\0' && (s)[11] == '\0' )
#define e_str12ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && e_tolower((d)[5]) == e_tolower(s[5]) && e_tolower((d)[6]) == e_tolower(s[6]) && e_tolower((d)[7]) == e_tolower(s[7]) && e_tolower((d)[8]) == e_tolower(s[8]) && e_tolower((d)[9]) == e_tolower(s[9]) && e_tolower((d)[10]) == e_tolower(s[10]) && e_tolower((d)[11]) == e_tolower(s[11]) && (d)[12] == '\0' && (s)[12] == '\0' )
#define e_str13ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && e_tolower((d)[5]) == e_tolower(s[5]) && e_tolower((d)[6]) == e_tolower(s[6]) && e_tolower((d)[7]) == e_tolower(s[7]) && e_tolower((d)[8]) == e_tolower(s[8]) && e_tolower((d)[9]) == e_tolower(s[9]) && e_tolower((d)[10]) == e_tolower(s[10]) && e_tolower((d)[11]) == e_tolower(s[11]) && e_tolower((d)[12]) == e_tolower(s[12]) && (d)[13] == '\0' && (s)[13] == '\0' )
#define e_str14ncmp(d, s) (e_tolower((d)[0]) == e_tolower(s[0]) && e_tolower((d)[1]) == e_tolower(s[1]) && e_tolower((d)[2]) == e_tolower(s[2]) && e_tolower((d)[3]) == e_tolower(s[3]) && e_tolower((d)[4]) == e_tolower(s[4]) && e_tolower((d)[5]) == e_tolower(s[5]) && e_tolower((d)[6]) == e_tolower(s[6]) && e_tolower((d)[7]) == e_tolower(s[7]) && e_tolower((d)[8]) == e_tolower(s[8]) && e_tolower((d)[9]) == e_tolower(s[9]) && e_tolower((d)[10]) == e_tolower(s[10]) && e_tolower((d)[11]) == e_tolower(s[11]) && e_tolower((d)[12]) == e_tolower(s[12]) && e_tolower((d)[13]) == e_tolower(s[13]) && (d)[14] == '\0' && (s)[14] == '\0' )

/* Some function for string usage. */
#define E_TO_CHAR(src, c, i) do {\
    for (; src[i++] != c; ); } while(0)
#define E_SKIP_CHAR(s, c, i) do { for ( ; s[(i)] == c; (i)++ );  } while(0)
#define E_TO_DIGIT(src, i) do {\
    for (;; ++i) { if (src[i] - '0' >= 0 && src[i] - '9' <= 9) break; } } while(0)
#define E_NOT_DIGIT(src, i) do {\
    for (;; ++i) { if (src[i] - '0' < 0 || src[i] - '9' > 9) break; } } while(0)
#define E_PARSE_CRON(f) do { for ( pr = k = p; ; ++k ) {if ( period[k] == ' ' ) {if ( is_co && !is_sl ) {min = (int)strtol(period + pr, NULL, 10); max = (int)strtol(period + (is_co + 1), NULL, 10); if ( time_v->f >= min && time_v->f <= max ) { return 1; } } else if ( !is_co && is_sl ) {min = (int)strtol(period + is_sl + 1, NULL, 10); if ( time_v->f % min == 0 ) { return 1; } } else {min = (int)strtol(period + pr, NULL, 10); if ( time_v->f == min ) return 1; } break; } else if ( period[k] == ',' ) {if ( is_co && !is_sl ) {min = (int)strtol(period + pr, NULL, 10); max = (int)strtol(period + (is_co + 1), NULL, 10); E_NOT_DIGIT(period, k); pr = k + 1; if ( time_v->f >= min && time_v->f <= max ) { return 1; } is_co = 0; } else if ( !is_co && is_sl ) {min = (int)strtol(period + is_sl + 1, NULL, 10); if ( time_v->f % min == 0 ) { return 1; } is_sl = 0; pr = k + 1; } else {min = (int)strtol(period + pr, NULL, 10); E_NOT_DIGIT(period, k); pr = k + 1; if ( time_v->f == min ) return 1; } } else if ( period[k] == '-' ) { is_co = k; } else if ( period[k] == '/' ) { is_sl = k; } } }while(0)
typedef struct _CSTRING
{
char    *s;
unsigned long long l; /* string length */
unsigned long long t; /* string's allocated memory */
} CSTRING;

CSTRING *new_cstring();
int new_cstring_add_long_long(CSTRING *c, long long i);
int new_cstring_add_double(CSTRING *c, double d);
int new_cstring_add_string(CSTRING *c, char *s, long sl);
int new_cstring_add_char(CSTRING *c, char v);
void new_cstring_free(CSTRING *c);
int new_cstring_0(CSTRING *c);


char *e_substr(char *src, unsigned long len);
long  e_concat_str(char **dest, char *src, unsigned long len, int _res);
void  e_parse_range(const char *str, long *from, long *to);
char *e_size_data_from_file(char *file, long from, long to);
char *e_data_from_file(char *file, long *size);
int   e_parse_second(struct tm *time_v, char *period, int *n);
void  e_write_to_file(char *file, char *fmt, ...);
int   e_parse_minute(struct tm *time_v,char *period, int *n);
int   e_parse_hour(struct tm *time_v,char *period, int *n);
int   e_parse_day(struct tm *time_v,char *period, int *n);
int   e_parse_month(struct tm *time_v,char *period, int *n);
int   e_parse_week(struct tm *time_v,char *period, int *n);
int   e_match_time(char *period);

#endif /* LISTS_ESTRING_H */
