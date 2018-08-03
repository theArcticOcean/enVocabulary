/**********************************************************
*
* @brief    The file has interfaces of Model module in
*           MVC design pattern.
*
* @author   theArcticOcean
***********************************************************/

#include "controller.h"
#include "log.h"

Controller* Controller::instance = NULL;
pthread_mutex_t Controller::mutex = PTHREAD_MUTEX_INITIALIZER;

/*
*   Constructor of class Controller
*/
Controller::Controller()
{
    UIMgr = NULL;
    pthread_mutex_init(&mutex, NULL);
}

/*
*   Destructor of class Controller
*/
Controller::~Controller()
{
    if(NULL != instance){
        delete instance;
        instance = NULL;
    }
}

/*
*   Get view manager object pointer.
*/
void Controller::attachViewManager(viewManager *UIMgr)
{
    this->UIMgr = UIMgr;
}

/*
*   Connect signals and slots to notify UI Manager to update HMI.
*/
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

    ret = connect(this,SIGNAL(internetConnectNotResponse()),UIMgr,SLOT(slotInternetConnectNotResponse()));
    LOGDBG("connected: %d for internetConnectNotResponse, UIMgr is %p", ret, UIMgr);

    ret = connect(this,SIGNAL(wordNotFound()),UIMgr,SLOT(slotWordNotFound()));
    LOGDBG("connected: %d for wordNotFound, UIMgr is %p", ret, UIMgr);

    ret = connect(UIMgr->searchUI->getHttpManager()->getRequestTimerRef(), &QTimer::timeout,
                  UIMgr,&viewManager::slotInternetConnectNotResponse);
    LOGDBG("connected: %d for timeout, UIMgr is %p", ret, UIMgr);
}

/*
*   Public interface provided to send view message to UIMgr.
*/
void Controller::sendViewMsg(viewMsgEnum msg)
{
    LOGDBG("start, msg is %d", msg);
    switch(msg)
    {
    case SearchWord:
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
    case InternetConnectNotResponse:
        emit internetConnectNotResponse();
        break;
    case WordNotFound:
        emit wordNotFound();
        break;
    default:
        break;
    }
    LOGDBG("end!");
}

/*
*   For user moving software, notifies UIMgr to update HMI.
*/
void Controller::sendMoveViewSignal(srcEnum e, QPoint newPoint)
{
    emit moveEventHappenedSig(e, newPoint);
}

/*
*   Get singleton pointer for Controller.
*   In fact, our software has not multi-threads mechanism, the mutex added is just for practice.
*/
Controller *Controller::getInstance()
{
    if(NULL == instance){
        pthread_mutex_lock(&mutex);
        instance = new Controller();
        pthread_mutex_unlock(&mutex);
    }
    return instance;
}
