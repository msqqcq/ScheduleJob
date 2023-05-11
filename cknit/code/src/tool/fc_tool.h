/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#ifndef FASTCJSON_FC_TOOL_H
#define FASTCJSON_FC_TOOL_H

#include <config.h>
#include <estring.h>
#include <log.h>
#include <log.h>
#include <help.h>

#if FC_DEBUG
    #define FC_ERROR_EXIT(...) do { _logger(LOG_ERROR, __VA_ARGS__); fprintf(stderr, __VA_ARGS__); exit(0); } while(0)
#else
    #define FC_ERROR_EXIT(...) do { _logger(LOG_ERROR, __VA_ARGS__); } while(0)
#endif

CJSON_FIELD_DEF(tasks)
    *status , *id,
    *command, *period
CJSON_FIELD_DEF_END(tasks);
CJSON_FIELD_FUNC_DEF(tasks);

CJSON_FIELD_DEF(taskId)
    *id
CJSON_FIELD_DEF_END(taskId);
CJSON_FIELD_FUNC_DEF(taskId);

CJSON_FIELD_DEF(stask)
    *daemon, *data, *task_conf 
CJSON_FIELD_DEF_END(stask);
CJSON_FIELD_FUNC_DEF(stask);

#endif /* FASTCJSON_FC_TOOL_H */
