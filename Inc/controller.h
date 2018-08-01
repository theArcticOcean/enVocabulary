/**********************************************************
*
* @brief    The file defines interfaces of Model module in
*           MVC design pattern.
*
* @author   theArcticOcean
***********************************************************/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "viewmanager.h"
#include <QObject>
#include <pthread.h>
#include "endata.h"

/*
*   Defines view message types, controller would emit different
*   signal based on these enum variables.
*/
typedef enum _viewMsgEnum{
    SearchWord,
    CloseAllUI,
    GotoWordSentences,
    GotoCollectSen,
    GotoCollectWord,
    SentenceWinToHome,
    CollectSenWndToHome,
    CollectWordWndToHome,
    AccessTokenInvalid,
    WordNotFound,
    EnumMax
}viewMsgEnum;

class Controller : public QObject
{
    Q_OBJECT

    static Controller* instance;
    static pthread_mutex_t mutex;
    viewManager *UIMgr;

public:
    Controller();
    ~Controller();
    void attachViewManager(viewManager *UIMgr);
    void signalAndSlotsConenct();
    void sendViewMsg(viewMsgEnum msg);
    void sendMoveViewSignal(srcEnum e, QPoint newPoint);
    static Controller* getInstance();

signals:
    void getWord();
    void closeAllUI();
    void gotoWordSentencesWnd();
    void gotoCollectSenWnd();
    void gotoCollectWordWnd();
    void SentenceWinToHomeWnd();
    void collectSenWndToHomeWnd();
    void collectWordWndToHomeWnd();
    void moveEventHappenedSig(srcEnum e, QPoint newPoint);
    void updateCollectSentencePage();
    void accessTokenInvalid();
    void wordNotFound();
};

#endif // CONTROLLER_H
