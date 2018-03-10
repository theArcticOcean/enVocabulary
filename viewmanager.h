#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QObject>
#include <search.h>
#include <mysentences.h>
#include <collectsentences.h>
#include <boost/smart_ptr.hpp>

class viewManager : public QObject
{
    Q_OBJECT
public:
    boost::shared_ptr<Search> searchUI;
    boost::shared_ptr<mySentences> mySentencesUI;
    boost::shared_ptr<collectSentences> collectSentencesUI;
public:
    explicit viewManager(QObject *parent = nullptr);

signals:

public slots:
    void slotCloseAllUI();
    void slotGotoWordSentencesWnd();
    void slotGotoCollectSenWnd();
    void slotSentenceWinToHomeWnd();
    void slotCollectSenWndToHomeWnd();
};

#endif // VIEWMANAGER_H
