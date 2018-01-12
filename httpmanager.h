#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <vector>
#include <map>
#include "endata.h"
using namespace std;

class HttpManager : public QObject
{
    Q_OBJECT
    const int buffer_len;
    int buffer_pos;
    enData *model;
public:
    explicit HttpManager(QObject *parent = nullptr);
    ~HttpManager();

    bool searchFilesMap(string url);
    static string getIncomLocalPicPath(string url);
    static string getComLocalPicPath(string url);
    bool sendPicRequest(char *url);
    bool sendEnWordSearch(char *word);

    QNetworkReply* netPicReply;
    QNetworkReply* netWordReply;
    QNetworkAccessManager *manager;
    // store url and pic files fds. if pic downloading is not completed,
    // we delete it in loading time at next time. when download pic finished,
    // rename picture as 'pic_***.png'
    map<string, int> filesMap;
    char *buffer;
signals:

public slots:
    void slotPicReadyRead();
    void slotPicFinished();
    void slotEnWordReadyRead();
    void slotEnWordFinished();
    void slotError(enum QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError> list);
};

#endif // HTTPMANAGER_H
