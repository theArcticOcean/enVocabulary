#include "controller.h"
#include "log.h"

Controller* Controller::instance = NULL;
pthread_mutex_t Controller::mutex = PTHREAD_MUTEX_INITIALIZER;

Controller::Controller()
{
    UIMgr = NULL;
    pthread_mutex_init(&mutex, NULL);
}

Controller::~Controller()
{
    if(NULL != instance){
        delete instance;
        instance = NULL;
    }
}

void Controller::attachViewManager(viewManager *UIMgr)
{
    this->UIMgr = UIMgr;
}

void Controller::signalAndSlotsConenct()
{
    bool ret = connect(this,SIGNAL(getWord()),UIMgr->searchUI.get(),SLOT(slotGetWord()));
    LOGDBG("connected: %d for getWord(), UIMgr is %p", ret, UIMgr);
    ret = connect(this,SIGNAL(closeAllUI()),UIMgr,SLOT(slotCloseAllUI()));
    LOGDBG("connected: %d for closeAllUI(), UIMgr is %p", ret, UIMgr);
    ret = connect(this,SIGNAL(gotoWordSentencesWnd()),UIMgr,SLOT(slotGotoWordSentencesWnd()));
    LOGDBG("connected: %d for gotoWordSentencesWnd(), UIMgr is %p", ret, UIMgr);
    ret = connect(this,SIGNAL(SentenceWinToHomeWnd()),UIMgr,SLOT(slotSentenceWinToHomeWnd()));
    LOGDBG("connected: %d for SentenceWinToHomeWnd(), UIMgr is %p", ret, UIMgr);
    ret = connect(this,SIGNAL(updateCollectSentencePage()),UIMgr,SLOT(slotUpdateCollectSentencePage()));
    LOGDBG("connected: %d for updateCollectSentencePage(), UIMgr is %p", ret, UIMgr);
    ret = connect(this,SIGNAL(gotoCollectSenWnd()), UIMgr, SLOT(slotGotoCollectSenWnd()));
    LOGDBG("connected: %d for gotoCollectSenWnd(), UIMgr is %p", ret, UIMgr);
    ret = connect(this,SIGNAL(CollectSenWndToHomeWnd()), UIMgr, SLOT(slotCollectSenWndToHomeWnd()));
    LOGDBG("connected: %d for CollectSenWndToHomeWnd(), UIMgr is %p", ret, UIMgr);
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
    case UpdateCollectSenPage:
        emit updateCollectSentencePage();
        break;
    case GotoCollectSen:
        emit gotoCollectSenWnd();
        break;
    case CollectSenWndToHome:
        emit CollectSenWndToHomeWnd();
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
