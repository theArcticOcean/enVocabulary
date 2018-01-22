#include "viewmanager.h"
#include "log.h"

viewManager::viewManager(QObject *parent) : QObject(parent)
{
    searchUI = new Search();
    searchUI->setAttribute(Qt::WA_DeleteOnClose, true);
    mySentencesUI = new mySentences();
    mySentencesUI->setAttribute(Qt::WA_DeleteOnClose, true);

    searchUI->show();
}

void viewManager::slotCloseAllUI()
{
    if(NULL != searchUI){
        searchUI->close();
    }
    if(NULL != mySentencesUI){
        mySentencesUI->close();
    }
}

void viewManager::slotGotoWordSentencesWnd()
{
    LOGDBG("start");
    //if(NULL != searchUI) searchUI->hide();
    //if(NULL != mySentencesUI) mySentencesUI->show();
    LOGDBG("end!");
}
