/**********************************************************
*
* @brief    The source file has detail of log print function
*           that is used in our project.
*
* @author   theArcticOcean
***********************************************************/

#include "log.h"
#include <stdio.h>
#include <unistd.h>

/*
*   The function is base of printing log.
*/
void LOGBASE(const char *file, const int line, const char *funcName, const char * fmt, ...)
{
    #define BUFFSIZE    1024*16
    char buffer[BUFFSIZE] = {0};
    int n = sprintf(buffer,"[file: %s, line: %d, funcName: %s ] ",file, line, funcName);

    va_list vap;
    va_start(vap, fmt);                         //variable arguments list vap init
    vsnprintf(buffer+n, BUFFSIZE-n, fmt, vap);  // add list vap to buff
    va_end(vap);

#ifdef DEBUG
    qDebug() << buffer;
#endif
}
