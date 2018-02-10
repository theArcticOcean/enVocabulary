#include "viewmanager.h"
#include "log.h"

viewManager::viewManager(QObject *parent) : QObject(parent)
{
    searchUI = boost::make_shared<Search>();
    //searchUI.get()->setAttribute(Qt::WA_DeleteOnClose, true);
    mySentencesUI = boost::make_shared<mySentences>();
    //mySentencesUI.get()->setAttribute(Qt::WA_DeleteOnClose, true);

    searchUI.get()->show();
}

void viewManager::slotCloseAllUI()
{
    LOGDBG("start");
    searchUI.get()->close();
    mySentencesUI.get()->close();
    LOGDBG("end!");
}

void viewManager::slotGotoWordSentencesWnd()
{
    LOGDBG("start");
    searchUI.get()->hide();
    mySentencesUI.get()->show();
    LOGDBG("end!");
}

void viewManager::slotSentenceWinToHomeWnd()
{
    LOGDBG("start");
    mySentencesUI.get()->hide();
    searchUI.get()->show();
    LOGDBG("end!");
}
