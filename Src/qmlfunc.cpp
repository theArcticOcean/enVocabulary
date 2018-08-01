/**********************************************************
*
* @brief    The file contains details of class QmlFunc which
*           provides responsing functions for click event of
*           rect.qml.
*
* @author   theArcticOcean
***********************************************************/

#include "qmlfunc.h"
#include "httpmanager.h"
#include "controller.h"
#include "log.h"

/*
*   Constructor of class QmlFunc.
*/
QmlFunc::QmlFunc(QObject *parent) : QObject(parent)
{

}

/*
*   Slot function of sentence area click.
*/
void QmlFunc::onClickSentenceArea()
{
    LOGDBG("start");
    HttpManager::getInstance()->sendEnWordSentenceRequest();
    LOGDBG("end!");
}

/*
*   Slot function of collected words area click.
*/
void QmlFunc::onClickCollectWordArea()
{
    Controller::getInstance()->sendViewMsg(GotoCollectWord);
}

/*
*   Slot function of collected sentences area click.
*/
void QmlFunc::onClickCollectSentenceArea()
{
    Controller::getInstance()->sendViewMsg(GotoCollectSen);
}
