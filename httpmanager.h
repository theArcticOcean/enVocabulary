#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <vector>
#include <map>
using namespace std;

class HttpManager : public QObject
{
    Q_OBJECT
public:
    explicit HttpManager(QObject *parent = nullptr);
    bool searchFilesMap(string url);
    static string getIncomLocalPicPath(string url);
    static string getComLocalPicPath(string url);
    bool sendPicRequest(char *url);

    QNetworkReply* netPicReply;
    QNetworkAccessManager *manager;
    // store url and pic files fds. if pic downloading is not completed,
    // we delete it in loading time at next time. when download pic finished,
    // rename picture as 'pic_***.png'
    map<string, int> filesMap;
signals:

public slots:
    void slotPicReadyRead();
    void slotPicFinished();
    void slotError(enum QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError> list);
};

#endif // HTTPMANAGER_H
