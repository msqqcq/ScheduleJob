%{
/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 * This file is the fastcjson's grammer parsing file, use the bison
 * to generate the grammer data, you can't modify it without annocement
 * if want, send email to me. Thank you.
 * bison -d fc_grammer.y -o fc_grammer.c
 */
    #include <stdio.h>
    #include <fc_json.h>
    #include <estring.h>
    #include <fc_stack.h>
    static char error_msg[50];
    int  _cjson_status;
    extern FILE *yyin;
    extern int yylex();
    extern int yylineno;
    extern void yyerror(char *);
    extern int yylex_destroy();
    extern int yyparse();
    typedef struct yy_buffer_state * YY_BUFFER_STATE;
    extern YY_BUFFER_STATE yy_scan_string(char * str);
    extern void yy_delete_buffer(YY_BUFFER_STATE buffer);

/**
 * @brief NOTICE
 * Some Global vars to store the data from JSON structure
 */
    FCS        *stack;
    FAST_CJSON *cjson, *tjson;
%}

%union
{
    struct {
    	long long ival;
    	double    dval;
    	char     *sval;
    	char      vtyp;
    } val;
}

%token<val> T_INT T_STR T_DOUBLE
%type<val>  object array keypair array_data value

%%
json
    :  object
    |  array
;

object
    : '{' keypair '}' {  }
    | '{' '}'
    {
	cjson = new_fast_cjson(CJSON_OBJECT);
	fcs_push(stack, cjson, NULL);
    }
;

keypair
    : T_STR ':' value
    {
    	switch( $3.vtyp )
    	{
    	    case CJSON_INT:
    	    	cjson = new_fast_cjson(CJSON_OBJECT);
		fcs_push(stack, cjson, NULL);
		cjson_object_add_int(cjson, $1.sval, strlen($1.sval), $3.ival);
    	    	break;
	    case CJSON_BOOL:
	    case CJSON_NULL:
	    	cjson = new_fast_cjson(CJSON_OBJECT);
		fcs_push(stack, cjson, NULL);
		cjson_object_add_int_tag(cjson, $1.sval, strlen($1.sval), $3.ival, $3.vtyp);
		break;
    	    case CJSON_DOUBLE:
		cjson = new_fast_cjson(CJSON_OBJECT);
		fcs_push(stack, cjson, NULL);
		cjson_object_add_double(cjson, $1.sval, strlen($1.sval), $3.dval);
    	    	break;
    	    case CJSON_STRING:
    	    	cjson = new_fast_cjson(CJSON_OBJECT);
		fcs_push(stack, cjson, NULL);
		cjson_object_add_string(cjson, $1.sval, strlen($1.sval), $3.sval, strlen($3.sval));
		free($3.sval);
    	    	break;
    	    case CJSON_ARRAY:
		tjson = fcs_pop(stack);
    	    	cjson = new_fast_cjson(CJSON_OBJECT);
		fcs_push(stack, cjson, NULL);
		cjson_object_add_cjson(cjson, $1.sval, strlen($1.sval), tjson, CJSON_ARRAY);
    	    	break;
    	    case CJSON_OBJECT:
		tjson = fcs_pop(stack);
    	    	cjson = new_fast_cjson(CJSON_OBJECT);
		fcs_push(stack, cjson, NULL);
		cjson_object_add_cjson(cjson, $1.sval, strlen($1.sval), tjson, CJSON_OBJECT);
    	    	break;
    	}
    	free($1.sval);
    }
    | keypair ',' T_STR ':' value
    {
	switch( $5.vtyp )
    	{
    	    case CJSON_INT:
    	    	cjson_object_add_int(cjson, $3.sval, strlen($3.sval), $5.ival);
    	    	break;
	    case CJSON_BOOL:
	    case CJSON_NULL:
	    	cjson_object_add_int_tag(cjson, $3.sval, strlen($3.sval), $5.ival, $5.vtyp);
	    	break;
    	    case CJSON_DOUBLE:
    	    	cjson_object_add_double(cjson, $3.sval, strlen($3.sval), $5.dval);
    	    	break;
    	    case CJSON_STRING:
    	    	cjson_object_add_string(cjson, $3.sval, strlen($3.sval), $5.sval, strlen($5.sval));
    	    	free($5.sval);
    	    	break;
    	    case CJSON_ARRAY:
		cjson = FCN_DATA_P(FCN_NEXT_P(FCS_HEAD_P(stack)));
		tjson = fcs_pop(stack);
		cjson_object_add_cjson(cjson, $3.sval, strlen($3.sval), tjson, CJSON_ARRAY);
    	    	break;
    	    case CJSON_OBJECT:
            	cjson = FCN_DATA_P(FCN_NEXT_P(FCS_HEAD_P(stack)));
    	    	tjson = fcs_pop(stack);
            	cjson_object_add_cjson(cjson, $3.sval, strlen($3.sval), tjson, CJSON_OBJECT);
    	    	break;
    	}
    	free($3.sval);
    }
;

array
    : '[' array_data ']' { }
    | '[' ']'
    {
	cjson = new_fast_cjson(CJSON_ARRAY);
	fcs_push(stack, cjson, NULL);
    }
;

array_data
    : value
    {
	switch( $1.vtyp )
    	{
    	    case CJSON_INT:
		cjson = new_fast_cjson(CJSON_ARRAY);
		fcs_push(stack, cjson, NULL);
		cjson_array_add_int(cjson, $1.ival);
    	    	break;
	    case CJSON_BOOL:
	    case CJSON_NULL:
		cjson = new_fast_cjson(CJSON_ARRAY);
		fcs_push(stack, cjson, NULL);
		cjson_array_add_int_tag(cjson, $1.ival, $1.vtyp);
	    	break;
    	    case CJSON_DOUBLE:
		cjson = new_fast_cjson(CJSON_ARRAY);
		fcs_push(stack, cjson, NULL);
		cjson_array_add_double(cjson, $1.dval);
    	    	break;
    	    case CJSON_STRING:
		cjson = new_fast_cjson(CJSON_ARRAY);
		fcs_push(stack, cjson, NULL);
		cjson_array_add_string(cjson, $1.sval, strlen($1.sval));
		free($1.sval);
    	    	break;
    	    case CJSON_ARRAY:
    	    	tjson = fcs_pop(stack);
    	    	cjson = new_fast_cjson(CJSON_ARRAY);
		fcs_push(stack, cjson, NULL);
		cjson_array_add_cjson(cjson, tjson, CJSON_ARRAY);
    	    	break;
    	    case CJSON_OBJECT:
    	    	tjson = fcs_pop(stack);
		cjson = new_fast_cjson(CJSON_ARRAY);
		fcs_push(stack, cjson, NULL);
		cjson_array_add_cjson(cjson, tjson, CJSON_OBJECT);
    	    	break;
    	}
    }
    | array_data ',' value
    {
	switch( $3.vtyp )
    	{
    	    case CJSON_INT:
    	    	cjson_array_add_int(cjson, $3.ival);
    	    	break;
	    case CJSON_BOOL:
	    case CJSON_NULL:
	    	cjson_array_add_int_tag(cjson, $3.ival, $3.vtyp);
	    	break;
    	    case CJSON_DOUBLE:
    	    	cjson_array_add_double(cjson, $3.dval);
    	    	break;
    	    case CJSON_STRING:
    	    	cjson_array_add_string(cjson, $3.sval, strlen($3.sval));
    	    	free($3.sval);
    	    	break;
    	    case CJSON_ARRAY:
    	    	cjson = FCN_DATA_P(FCN_NEXT_P(FCS_HEAD_P(stack)));
    	    	tjson = fcs_pop(stack);
    	    	cjson_array_add_cjson(cjson, tjson, CJSON_ARRAY);
    	    	break;
    	    case CJSON_OBJECT:
		cjson = FCN_DATA_P(FCN_NEXT_P(FCS_HEAD_P(stack)));
    	    	tjson = fcs_pop(stack);
		cjson_array_add_cjson(cjson, tjson, CJSON_OBJECT);
    	    	break;
    	}
    }
;

value
    :  T_STR
    {
	$$ = $1;
    }
    |  T_INT
    {
	$$ = $1;
    }
    |  T_DOUBLE
    {
	$$ = $1;
    }
    | array
    {
	$$.vtyp = CJSON_ARRAY;
    }
    | object
    {
	$$.vtyp = CJSON_OBJECT;
    }
;

%%

void fastcjson_error(char *s)
{
    e_memzero(error_msg, sizeof(error_msg));
    sprintf(error_msg, "Error Token: %s in line: %d", s, yylineno);
    _cjson_status = 0;
}

void yyerror(char *err)
{
    fastcjson_error(err);
}

char *cjson_get_error()
{
    if ( !_cjson_status ) { return error_msg; }
    return NULL;
}

FAST_CJSON *decode_cjson(char *json_string)
{
    _cjson_status = 1;
    YY_BUFFER_STATE buffer;

    stack = new_fcs();
    buffer = yy_scan_string(json_string);
    yyparse();
    yy_delete_buffer(buffer);

    fcs_destroy(stack);
    yylex_destroy();

    if ( !_cjson_status ) {
     	fast_cjson_destroy(cjson); return NULL;
    }
    return cjson;
}