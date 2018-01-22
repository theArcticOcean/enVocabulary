#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <QObject>
#include <search.h>
#include <mysentences.h>

class viewManager : public QObject
{
    Q_OBJECT
public:
    Search *searchUI;
    mySentences *mySentencesUI;

public:
    explicit viewManager(QObject *parent = nullptr);

signals:

public slots:
    void slotCloseAllUI();
    void slotGotoWordSentencesWnd();
};

#endif // VIEWMANAGER_H
