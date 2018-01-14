#include "controller.h"
#include "log.h"

Controller* Controller::instance = NULL;
pthread_mutex_t Controller::mutex = PTHREAD_MUTEX_INITIALIZER;

Controller::Controller()
{
    wordSearch = NULL;
    pthread_mutex_init(&mutex, NULL);
}

void Controller::attachSearch(Search *wSearch)
{
    wordSearch = wSearch;
}

void Controller::signalAndSlotsConenct()
{
    bool ret = connect(this,SIGNAL(getWord()),wordSearch,SLOT(slotGetWord()));
    LOGDBG("connected: %d, wordSearch is %p", ret, wordSearch);
}

void Controller::sendViewMsg(viewMsgEnum msg)
{
    LOGDBG("start, msg is %d", msg);
    switch(msg) {
    case WordEnum:
        emit getWord();
        LOGDBG("getword signal sent, wordSearch is %p", wordSearch);
        break;
    default:
        break;
    }
    LOGDBG("end!");
}

Controller *Controller::getInstance()
{
    if(NULL == instance){
        pthread_mutex_lock(&mutex);
        instance = new Controller();
        pthread_mutex_unlock(&mutex);
    }
    return instance;
}
