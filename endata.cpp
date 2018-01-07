#include "endata.h"
#include "log.h"
#include <queue>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;

pthread_mutex_t enData::instanceMutex = PTHREAD_MUTEX_INITIALIZER;
enData* enData::instance = NULL;

enData::enData()
{
    pthread_mutex_init(&instanceMutex, NULL);
    if(0 != access(DOWNFILES_PATH, F_OK)){
        if(0 != mkdir(DOWNFILES_PATH, 0777)){
            LOGDBG("mkdir failed: %s", strerror(errno));
        }
    }
    if(0 != access(DOWNFILES_PICS_PATH, F_OK)){
        if(0 != mkdir(DOWNFILES_PICS_PATH, 0777)){
            LOGDBG("mkdir failed: %s", strerror(errno));
        }
    }
    picsCheck();
}

/*
* @brief    it works at program start, check the number of pictures, make sure it is less than
*           equal to MAX_PICS_NUMBER
* @author   weiyang
* @date     2017.12.31
*/
void enData::picsCheck()
{
    DIR *dir = opendir(DOWNFILES_PICS_PATH);
    if(NULL == dir){
        LOGDBG("opendir failed: %s", strerror(errno));
        return ;
    }
    struct dirent *entry;
    int number;
    queue<char *> files;         // front === back

    number = 0;
    entry = readdir(dir);
    while(NULL != entry){
        if(entry->d_name[0] == '.'){
            entry = readdir(dir);
            continue;
        }
        if(strlen(entry->d_name) > 4 && 0 != strncmp(entry->d_name,"pic_",4)){
            unlink(entry->d_name);   //delete incomplete files.
        }
        number++;
        files.push(entry->d_name);
        entry = readdir(dir);
    }

    while(number > MAX_PICS_NUMBER){
        unlink(files.front());
        files.pop();
        number--;
    }
}

enData *enData::getInstance()
{
    if(NULL == instance){
        pthread_mutex_lock(&instanceMutex);
        instance = new enData();
        pthread_mutex_unlock(&instanceMutex);
    }
    return instance;
}
