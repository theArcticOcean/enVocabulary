#include "viewmanager.h"
#include "log.h"
#include <QMessageBox>
#include <QDesktopServices>

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

void viewManager::slotMoveEventHappened(srcEnum e, QPoint newPoint)
{
    LOGDBG("start, srcEnum is %d", e);
    searchUI.get()->move(newPoint);
    collectSentencesUI.get()->move(newPoint);
    collectWordsUI.get()->move(newPoint);
    mySentencesUI.get()->move(newPoint);
    LOGDBG("end!");
}

void viewManager::slotAccessTokenInvalid()
{
    int ret;
    ret = QMessageBox::warning(searchUI.get(),
                         "notice",
                         "Your access_token is no longer valid for remote server.\n"
                         "Please contact with software enginer to solve the problem.",
                         QMessageBox::Ok,
                         QMessageBox::Help);
    if(QMessageBox::Help == ret)
    {
        QDesktopServices::openUrl(QUrl("http://weiy.org/2018/04/07/envocabulary/"));
    }
}

void viewManager::slotWordNotFound()
{
    QMessageBox::warning(searchUI.get(),
                             "notice",
                             "Your word is not found by Shanbay API.\n"
                             "Please enter another valid English word to look up.",
                             QMessageBox::Ok);
}
