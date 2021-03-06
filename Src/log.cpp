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
#include <QCoreApplication>

/*
*   The function is base of printing log.
*/
void LOGBASE(const char *file, const int line, const char *funcName, const char * fmt, ...)
{   
    #define BUFFSIZE    1024*16
    char buffer[BUFFSIZE] = {0};
    int n = sprintf(buffer,"[file: %s, line: %d, funcName: %s ]\n",file, line, funcName);

    va_list vap;
    va_start(vap, fmt);                         //variable arguments list vap init
    vsnprintf(buffer+n, BUFFSIZE-n, fmt, vap);  // add list vap to buff
    va_end(vap);

    FILE *fp;
    fp = fopen( logFilePath.toStdString().c_str(), "a" );
    if( NULL != fp )
    {
        fprintf( fp, "%s\n", buffer );
        fclose( fp );
    }
}

/*
*   Create log file, truncate the file if it exsits.
*/
void LogInit()
{
    logFilePath = QCoreApplication::applicationDirPath();
    logFilePath = logFilePath + "/log.txt";
    if(  0 == access( logFilePath.toStdString().c_str(), F_OK )
         && truncate( logFilePath.toStdString().c_str(), 0 ) )
    {
        QString prefix = "[logInit] ";
        prefix = prefix + logFilePath;
        perror( prefix.toStdString().c_str() );
    }
}
