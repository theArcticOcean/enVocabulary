#include "viewmanager.h"
#include "log.h"

viewManager::viewManager(QObject *parent) : QObject(parent)
{
    searchUI = boost::make_shared<Search>();
    mySentencesUI = boost::make_shared<mySentences>();
    collectSentencesUI = boost::make_shared<collectSentences>();
    collectWordsUI = boost::make_shared<collectWords>();
    //mySentencesUI.get()->setAttribute(Qt::WA_DeleteOnClose, true);
    searchUI.get()->show();
}

void viewManager::slotCloseAllUI()
{
    LOGDBG("start");
    searchUI.get()->close();
    mySentencesUI.get()->close();
    collectSentencesUI.get()->close();
    collectWordsUI.get()->close();
    LOGDBG("end!");
}

void viewManager::slotGotoWordSentencesWnd()
{
    LOGDBG("start");
    searchUI.get()->hide();
    mySentencesUI.get()->show();
    LOGDBG("end!");
}

void viewManager::slotGotoCollectSenWnd()
{
    LOGDBG("start");
    searchUI.get()->hide();
    collectSentencesUI.get()->show();
    LOGDBG("end!");
}

void viewManager::slotGotoCollectWordWnd()
{
    LOGDBG("start");
    searchUI.get()->hide();
    collectWordsUI.get()->show();
    LOGDBG("end!");
}

void viewManager::slotSentenceWinToHomeWnd()
{
    LOGDBG("start");
    mySentencesUI.get()->hide();
    searchUI.get()->show();
    LOGDBG("end!");
}

void viewManager::slotCollectSenWndToHomeWnd()
{
    LOGDBG("start");
    collectSentencesUI.get()->hide();
    searchUI.get()->show();
    LOGDBG("end!");
}

void viewManager::slotCollectWordWndToHomeWnd()
{
    LOGDBG("start");
    collectWordsUI.get()->hide();
    searchUI.get()->show();
    LOGDBG("end!");
}
