#include "controller.h"
#include "log.h"

Controller* Controller::instance = NULL;
pthread_mutex_t Controller::mutex = PTHREAD_MUTEX_INITIALIZER;

Controller::Controller()
{
    UIMgr = NULL;
    pthread_mutex_init(&mutex, NULL);
}

void Controller::attachViewManager(viewManager *UIMgr)
{
    this->UIMgr = UIMgr;
}

void Controller::signalAndSlotsConenct()
{
    bool ret = connect(this,SIGNAL(getWord()),&UIMgr->searchUI,SLOT(slotGetWord()));
    ret = connect(this,SIGNAL(closeAllUI()),UIMgr,SLOT(slotCloseAllUI()));
    LOGDBG("connected: %d for closeAllUI(), UIMgr is %p", ret, UIMgr);
    ret = connect(this,SIGNAL(gotoWordSentencesWnd()),UIMgr,SLOT(slotGotoWordSentencesWnd()));
    LOGDBG("connected: %d for gotoWordSentencesWnd(), UIMgr is %p", ret, UIMgr);
    ret = connect(this,SIGNAL(SentenceWinToHomeWnd()),UIMgr,SLOT(slotSentenceWinToHomeWnd()));
}

void Controller::sendViewMsg(viewMsgEnum msg)
{
    LOGDBG("start, msg is %d", msg);
    switch(msg) {
    case SearchWordEnum:
        emit getWord();
        break;
    case CloseAllUI:
        emit closeAllUI();
        break;
    case GotoWordSentences:
        LOGDBG("UIMgr is %p", UIMgr);
        emit gotoWordSentencesWnd();
        break;
    case SentenceWinToHome:
        emit SentenceWinToHomeWnd();
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
