/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 * This file contains some functions for parsing the FAST_CJSON to Given
 * Data structure
 *
 * There are 4 pair macros to do this job:
 *
 * 1. CJSON_FIELD_DEF(name)
 *    CJSON_FIELD_DEF_END(name)
 *    This macro will generate the structure named: CJSON_name, so if you given the `nam`e with 'data',
 *    the data will generate to CJSON_data
 *
 * 2. CJSON_FIELD_FUNC_DEF(name)
 *    This macro used to generate the function declarations
 *
 * 3. CJSON_FIELD_FUNC(name)
 *    CJSON_FIELD_FUNC_END()
 *
 * 4. CJSON_FIELD_CMP(name, len, if/elif)
 *
 * NOTICE:
 * The CJSON_name's var has a `oth` var to decide the current JSON format okay(0) or not.(1)
 */

#include <fc_tool.h>
#include <estring.h>

CJSON_FIELD_FUNC(tasks)
    CJSON_FIELD_CMP(command, 7,   if)
    CJSON_FIELD_CMP(period,  6, elif)
    CJSON_FIELD_CMP(status,  6, elif)
    CJSON_FIELD_CMP(id,      2, elif)
CJSON_FIELD_FUNC_END()

CJSON_FIELD_FUNC(taskId)
    CJSON_FIELD_CMP(id,      2, if)
CJSON_FIELD_FUNC_END()

CJSON_FIELD_FUNC(stask)
    CJSON_FIELD_CMP(daemon,      6,   if)
    CJSON_FIELD_CMP(data,        4, elif)
    CJSON_FIELD_CMP(task_conf,   9, elif)
CJSON_FIELD_FUNC_END()
