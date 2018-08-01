/**********************************************************
*
* @brief    Class QmlFunc provides responsing function for
*           click event of rect.qml.
*
* @author   theArcticOcean
***********************************************************/

#ifndef QMLFUNC_H
#define QMLFUNC_H

#include <QObject>

class QmlFunc : public QObject
{
    Q_OBJECT
public:
    explicit QmlFunc(QObject *parent = nullptr);
public:
    Q_INVOKABLE void onClickSentenceArea();
    Q_INVOKABLE void onClickCollectWordArea();
    Q_INVOKABLE void onClickCollectSentenceArea();
signals:

public slots:
};

#endif // QMLFUNC_H
