#include "viewmanager.h"
#include "log.h"

viewManager::viewManager(QObject *parent) : QObject(parent)
{
    searchUI.setAttribute(Qt::WA_DeleteOnClose, true);
    mySentencesUI.setAttribute(Qt::WA_DeleteOnClose, true);
    searchUI.show();
}

void viewManager::slotCloseAllUI()
{
    searchUI.close();
    mySentencesUI.close();
}

void viewManager::slotGotoWordSentencesWnd()
{
    LOGDBG("start");
    searchUI.hide();
    mySentencesUI.show();
    LOGDBG("end!");
}

void viewManager::slotSentenceWinToHomeWnd()
{
    LOGDBG("start");
    mySentencesUI.hide();
    searchUI.show();
    LOGDBG("end!");
}
