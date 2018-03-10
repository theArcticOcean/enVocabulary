#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "viewmanager.h"
#include <QObject>
#include <pthread.h>

typedef enum _viewMsgEnum{
    SearchWordEnum,
    CloseAllUI,
    GotoWordSentences,
    GotoCollectSen,
    GotoCollectWord,
    SentenceWinToHome,
    CollectSenWndToHome,
    CollectWordWndToHome,
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
    static Controller* getInstance();

signals:
    void getWord();
    void closeAllUI();
    void gotoWordSentencesWnd();
    void gotoCollectSenWnd();
    void gotoCollectWordWnd();
    void SentenceWinToHomeWnd();
    void CollectSenWndToHomeWnd();
    void CollectWordWndToHomeWnd();
    void updateCollectSentencePage();
};

#endif // CONTROLLER_H
