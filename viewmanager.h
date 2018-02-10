#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QObject>
#include <search.h>
#include <mysentences.h>
#include <boost/smart_ptr.hpp>

class viewManager : public QObject
{
    Q_OBJECT
public:
    boost::shared_ptr<Search> searchUI;
    boost::shared_ptr<mySentences> mySentencesUI;

public:
    explicit viewManager(QObject *parent = nullptr);

signals:

public slots:
    void slotCloseAllUI();
    void slotGotoWordSentencesWnd();
    void slotSentenceWinToHomeWnd();
};

#endif // VIEWMANAGER_H
