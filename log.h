#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <QDebug>
#include <pthread.h>

void LOGBASE(const pthread_t tid, const char *file, const int line, const char *funcName, const char * fmt, ...);

#define  LOGDBG(fmt,args...)   LOGBASE(pthread_self(), __FILE__,__LINE__,__FUNCTION__, fmt, ##args)

#endif /* LOG_H_ */
