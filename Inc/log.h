/**********************************************************
*
* @brief    The header file contains log print function which
*           is used in our project.
*
* @author   theArcticOcean
***********************************************************/

#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <QDebug>
#include <pthread.h>

#define DEBUG 1

void LOGBASE(const char *file, const int line, const char *funcName, const char * fmt, ...);

#define  LOGDBG(fmt,args...)   LOGBASE(__FILE__,__LINE__,__FUNCTION__, fmt, ##args)

#endif /* LOG_H_ */
