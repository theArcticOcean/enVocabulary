#ifndef ENDATA_H
#define ENDATA_H

#define DOWNFILES_PATH          "./Down"
#define DOWNFILES_PICS_PATH     "./Down/Pics"
#define MAX_PICS_NUMBER          200

#include <pthread.h>

class enData
{    
    static enData *instance;
    static pthread_mutex_t instanceMutex;
public:
    enData();
    void picsCheck();
    static enData* getInstance();
};

#endif // ENDATA_H
