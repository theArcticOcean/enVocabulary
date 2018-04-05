#include "qmlfunc.h"
#include "httpmanager.h"
#include "controller.h"
#include "log.h"

QmlFunc::QmlFunc(QObject *parent) : QObject(parent)
{

}

void QmlFunc::onClickSentenceArea()
{
    LOGDBG("start");
    HttpManager::getInstance()->sendEnWordSentenceRequest();
    LOGDBG("end!");
}

void QmlFunc::onClickCollectWordArea()
{
    Controller::getInstance()->sendViewMsg(GotoCollectWord);
}

void QmlFunc::onClickCollectSentenceArea()
{
    Controller::getInstance()->sendViewMsg(GotoCollectSen);
}
