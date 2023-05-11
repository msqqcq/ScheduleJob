/**
 * Copyright @2019 Josin All Rights Reserved.
 * Author: Josin
 * Email : xeapplee@gmail.com
 */

#include <log.h>
#include <config.h>

void _logger(int type, char *str, ...)
{
/**
 * @brief Introduction
 * Write log info into the different file
 * Now support two types of file:
 * LOG_INFO :  Information log
 * LOG_ERROR:  Error log contains the kernel error or function invoking mistake
 */
    FILE   *fp = NULL;
    va_list list;
    
    switch(type)
    {
        case LOG_INFO:
            fp = fopen(INFO_LOG_NAME, "a+");
            break;
        case LOG_ERROR:
            fp = fopen(ERRO_LOG_NAME, "a+");
            break;
        case LOG_NOTICE:
            fp = fopen(NOTI_LOG_NAME, "a+");
            break;
        case LOG_WARNING:
            fp = fopen(WARN_LOG_NAME, "a+");
            break;
        case LOG_TASK:
            fp = fopen(TASK_LOG_NAME, "a+");
            break;
        case LOG_OPRA:
            fp = fopen(OPER_LOG_NAME, "a+");
            break;
        default:
            return ;
    }
    
    va_start(list, str);
    vfprintf(fp, str, list);
    va_end(list);
    
    fclose(fp);
}