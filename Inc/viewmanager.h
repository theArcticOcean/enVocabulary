/**********************************************************
*
* @brief    The file contains declaration of viewManager
*           that manages all windows of envocabulary.
*
* @author   theArcticOcean
***********************************************************/

#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QObject>
#include "search.h"
#include "mysentences.h"
#include "collectsentences.h"
#include "collectwords.h"
#include "endata.h"
#include <boost/smart_ptr.hpp>

class viewManager : public QObject
{
    Q_OBJECT
public:
    boost::shared_ptr<Search> searchUI;
    boost::shared_ptr<mySentences> mySentencesUI;
    boost::shared_ptr<collectSentences> collectSentencesUI;
    boost::shared_ptr<collectWords> collectWordsUI;
public:
    explicit viewManager(QObject *parent = nullptr);

signals:

public slots:
    void slotCloseAllUI();
    void slotGotoWordSentencesWnd();
    void slotGotoCollectSenWnd();
    void slotGotoCollectWordWnd();
    void slotSentenceWinToHomeWnd();
    void slotCollectSenWndToHomeWnd();
    void slotCollectWordWndToHomeWnd();
    void slotMoveEventHappened(srcEnum e, QPoint newPoint);
    void slotAccessTokenInvalid();
    void slotInternetConnectNotResponse();
    void slotWordNotFound();
};

#endif // VIEWMANAGER_H
