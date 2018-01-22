#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "viewmanager.h"
#include <QObject>
#include <pthread.h>

typedef enum _viewMsgEnum{
    SearchWordEnum,
    CloseAllUI,
    GotoWordSentences,
    EnumMax
}viewMsgEnum;

class Controller : public QObject
{
    Q_OBJECT
    viewManager *UIMgr;
    static Controller* instance;
    static pthread_mutex_t mutex;

public:
    Controller();
    void attachViewManager(viewManager *UIMgr);
    void signalAndSlotsConenct();
    void sendViewMsg(viewMsgEnum msg);
    static Controller* getInstance();

signals:
    void getWord();
    void closeAllUI();
    void gotoWordSentencesWnd();
};

#endif // CONTROLLER_H
