#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "search.h"
#include <QObject>
#include <pthread.h>

typedef enum _viewMsgEnum{
    WordEnum,
    EnumMax
}viewMsgEnum;

class Controller : public QObject
{
    Q_OBJECT
    Search *wordSearch;
    static Controller* instance;
    static pthread_mutex_t mutex;

public:
    Controller();
    void attachSearch(Search *wSearch);
    void signalAndSlotsConenct();
    void sendViewMsg(viewMsgEnum msg);
    static Controller* getInstance();
signals:
    void getWord();
};

#endif // CONTROLLER_H
