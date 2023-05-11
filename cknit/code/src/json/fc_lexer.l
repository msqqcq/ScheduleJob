%{
/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 * This is the fast_cjson lexer file, you are permitted only to view it, copy it ,but not modify
 * if you want to modify it, you must commit you changes to let me merge, Thank you very much.
 * flex -o fc_lexer.c fc_lexer.l
 */
    #include <fc_grammer.h>
    #include <fc_json.h>
    extern int yylex();
    extern int yylineno;
    extern FILE *yyin;
    extern void fastcjson_error(char *);

%}
%option noyywrap case-insensitive
%x COMMENT
%%

<INITIAL>"#"                                        { BEGIN COMMENT;             }
<INITIAL>"//"                                       { BEGIN COMMENT;             }
<COMMENT>.                                          {  }
<COMMENT>\n                                         { yylineno++; BEGIN INITIAL; }
<INITIAL>"{"                                        { return yytext[0];          }
<INITIAL>"}"                                        { return yytext[0];          }
<INITIAL>"["                                        { return yytext[0];          }
<INITIAL>"]"                                        { return yytext[0];          }
<INITIAL>":"                                        { return yytext[0];          }
<INITIAL>","                                        { return yytext[0];          }
<INITIAL>(-)?([1-9][0-9]*|[0-9])                    { yylval.val.vtyp = CJSON_INT;    yylval.val.ival = strtol(yytext, NULL, 10); return T_INT;       }
<INITIAL>(-)?([1-9][0-9]*|[0-9])?\.([0-9]*)?        { yylval.val.vtyp = CJSON_DOUBLE; yylval.val.dval = strtod(yytext, NULL); return T_DOUBLE;        }
<INITIAL>\"[^\"]*\"                                 { yylval.val.vtyp = CJSON_STRING; yylval.val.sval = strndup(yytext + 1, strlen(yytext+1) - 1); return T_STR;    }
<INITIAL>"on"                                       { yylval.val.vtyp = CJSON_BOOL;   yylval.val.ival = 1; return T_INT;    }
<INITIAL>"off"                                      { yylval.val.vtyp = CJSON_BOOL;   yylval.val.ival = 0; return T_INT;    }
<INITIAL>"true"                                     { yylval.val.vtyp = CJSON_BOOL;   yylval.val.ival = 1; return T_INT;    }
<INITIAL>"false"                                    { yylval.val.vtyp = CJSON_BOOL;   yylval.val.ival = 0; return T_INT;    }
<INITIAL>"null"                                     { yylval.val.vtyp = CJSON_NULL;   yylval.val.ival = 0; return T_INT;    }
<INITIAL>[ \t\r\f\v]                                {                            }
<INITIAL>\n                                         { yylineno++;                }
<INITIAL>.                                          { fastcjson_error(yytext);   }
%%