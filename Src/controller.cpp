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

    ret = connect(this,SIGNAL(gotoCollectSenWnd()), UIMgr, SLOT(slotGotoCollectSenWnd()));
    LOGDBG("connected: %d for gotoCollectSenWnd(), UIMgr is %p", ret, UIMgr);

    ret = connect(this,SIGNAL(gotoCollectWordWnd()), UIMgr, SLOT(slotGotoCollectWordWnd()));
    LOGDBG("connected: %d for gotoCollectWordWnd(), UIMgr is %p", ret, UIMgr);

    ret = connect(this,SIGNAL(collectSenWndToHomeWnd()), UIMgr, SLOT(slotCollectSenWndToHomeWnd()));
    LOGDBG("connected: %d for collectSenWndToHomeWnd(), UIMgr is %p", ret, UIMgr);

    ret = connect(this,SIGNAL(collectWordWndToHomeWnd()), UIMgr, SLOT(slotCollectWordWndToHomeWnd()));
    LOGDBG("connected: %d for collectWordWndToHomeWnd(), UIMgr is %p", ret, UIMgr);

    ret = connect(this,SIGNAL(moveEventHappenedSig(srcEnum,QPoint)), UIMgr, SLOT(slotMoveEventHappened(srcEnum,QPoint)));
    LOGDBG("connected: %d for moveEventHappenedSig, UIMgr is %p", ret, UIMgr);

    ret = connect(this,SIGNAL(accessTokenInvalid()),UIMgr,SLOT(slotAccessTokenInvalid()));
    LOGDBG("connected: %d for accessTokenInvalid, UIMgr is %p", ret, UIMgr);

    ret = connect(this,SIGNAL(wordNotFound()),UIMgr,SLOT(slotWordNotFound()));
    LOGDBG("connected: %d for wordNotFound, UIMgr is %p", ret, UIMgr);
}

void Controller::sendViewMsg(viewMsgEnum msg)
{
    LOGDBG("start, msg is %d", msg);
    switch(msg)
    {
    case SearchWordEnum:
        emit getWord();
        break;
    case CloseAllUI:
        emit closeAllUI();
        break;
    case GotoWordSentences:
        emit gotoWordSentencesWnd();
        break;
    case SentenceWinToHome:
        emit SentenceWinToHomeWnd();
        break;
    case GotoCollectSen:
        emit gotoCollectSenWnd();
        break;
    case CollectSenWndToHome:
        emit collectSenWndToHomeWnd();
        break;
    case GotoCollectWord:
        emit gotoCollectWordWnd();
        break;
    case CollectWordWndToHome:
        emit collectWordWndToHomeWnd();
        break;
    case AccessTokenInvalid:
        emit accessTokenInvalid();
        break;
    case WordNotFound:
        emit wordNotFound();
        break;
    default:
        break;
    }
    LOGDBG("end!");
}

void Controller::sendMoveViewSignal(srcEnum e, QPoint newPoint)
{
    emit moveEventHappenedSig(e, newPoint);
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
