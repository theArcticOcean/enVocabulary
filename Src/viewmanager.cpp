/**********************************************************
*
* @brief    The file contains definition of class viewManager.
*
* @author   theArcticOcean
***********************************************************/

#include "viewmanager.h"
#include "log.h"
#include <QMessageBox>
#include <QDesktopServices>

/*
*   Constructor of viewManager.
*/
viewManager::viewManager(QObject *parent) : QObject(parent)
{
    searchUI = boost::make_shared<Search>();
    mySentencesUI = boost::make_shared<mySentences>();
    collectSentencesUI = boost::make_shared<collectSentences>();
    collectWordsUI = boost::make_shared<collectWords>();
    //mySentencesUI.get()->setAttribute(Qt::WA_DeleteOnClose, true);
    searchUI.get()->show();
}

/*
*   Slot function for close all windows.
*/
void viewManager::slotCloseAllUI()
{
    LOGDBG("start");
    searchUI.get()->close();
    mySentencesUI.get()->close();
    collectSentencesUI.get()->close();
    collectWordsUI.get()->close();
    LOGDBG("end!");
}

/*
*   Slot function for user go to example sentences of special word.
*/
void viewManager::slotGotoWordSentencesWnd()
{
    LOGDBG("start");
    searchUI.get()->hide();
    mySentencesUI.get()->show();
    LOGDBG("end!");
}

/*
*   Slot function for user go to collected sentences window.
*/
void viewManager::slotGotoCollectSenWnd()
{
    LOGDBG("start");
    searchUI.get()->hide();
    collectSentencesUI.get()->show();
    LOGDBG("end!");
}

/*
*   Slot function for user go to collected words window.
*/
void viewManager::slotGotoCollectWordWnd()
{
    LOGDBG("start");
    searchUI.get()->hide();
    collectWordsUI.get()->show();
    LOGDBG("end!");
}

/*
*   Slot function for user go back to home page from example sentences window.
*/
void viewManager::slotSentenceWinToHomeWnd()
{
    LOGDBG("start");
    mySentencesUI.get()->hide();
    searchUI.get()->show();
    LOGDBG("end!");
}

/*
*   Slot function for user go back to home page from collected sentences window.
*/
void viewManager::slotCollectSenWndToHomeWnd()
{
    LOGDBG("start");
    collectSentencesUI.get()->hide();
    searchUI.get()->show();
    LOGDBG("end!");
}

/*
*   Slot function for user go back to home page from collected words window.
*/
void viewManager::slotCollectWordWndToHomeWnd()
{
    LOGDBG("start");
    collectWordsUI.get()->hide();
    searchUI.get()->show();
    LOGDBG("end!");
}

/*
*   Slot function for user move software action.
*/
void viewManager::slotMoveEventHappened(srcEnum e, QPoint newPoint)
{
    LOGDBG("start, srcEnum is %d", e);
    searchUI.get()->move(newPoint);
    collectSentencesUI.get()->move(newPoint);
    collectWordsUI.get()->move(newPoint);
    mySentencesUI.get()->move(newPoint);
    LOGDBG("end!");
}

/*
*   Slot function for dealing with the scene shanbay access token become invalid.
*/
void viewManager::slotAccessTokenInvalid()
{
    int ret;
    QMessageBox msgBox;

    msgBox.setText( "Your access_token is no longer valid for remote server.\n"
                    "Please contact with software enginer to solve the problem." );
    msgBox.setStandardButtons( QMessageBox::Ok | QMessageBox::Help );
    msgBox.setDefaultButton( QMessageBox::Help );
    ret = msgBox.exec();
    if( QMessageBox::Help == ret )
    {
        QDesktopServices::openUrl(QUrl("http://weiy.org/2018/04/07/envocabulary/"));
    }
    else {
        searchUI->setKeyOnMessageBox( true );
    }
}

/*
*   Slot function for dealing with the internet connection.
*/
void viewManager::slotInternetConnectNotResponse()
{
    int ret;
    QMessageBox msgBox;

    msgBox.setText( "Please check your internet connection." );
    msgBox.setStandardButtons( QMessageBox::Ok );
    ret = msgBox.exec();
    if( QMessageBox::Ok == ret )
    {
        searchUI->setKeyOnMessageBox( true );
    }
}

/*
*   Slot function deal with the scene request word is not found.
*/
void viewManager::slotWordNotFound()
{
    int ret;
    QMessageBox msgBox;
    msgBox.setText( "Your word is not found by Shanbay API.\n"
                    "Please enter another valid English word to look up." );
    msgBox.setStandardButtons( QMessageBox::Ok  );
    msgBox.setDefaultButton( QMessageBox::Ok );
    ret = msgBox.exec();
    if( QMessageBox::Ok == ret )
    {
        searchUI->setKeyOnMessageBox( true );
    }
//    QMessageBox::warning(searchUI.get(),
//                             "notice",
//                             "Your word is not found by Shanbay API.\n"
//                             "Please enter another valid English word to look up.",
//                             QMessageBox::Ok);
}
