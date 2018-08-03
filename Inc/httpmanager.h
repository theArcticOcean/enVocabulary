/**********************************************************
*
* @brief    HttpManager is responsible to request word's
*           information based on shanbay API.
*
* @author   theArcticOcean
***********************************************************/

#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSslSocket>
#include <vector>
#include <map>
#include <QTimer>
#include <boost/smart_ptr.hpp>
#include "endata.h"
using namespace std;

class HttpManager : public QObject
{
    Q_OBJECT

public:
    explicit HttpManager(QObject *parent = nullptr);
    ~HttpManager();

    bool searchFilesMap(string url);
    static string getIncomLocalPicPath(string url);
    static string getComLocalPicPath(string url);
    bool sendPicRequest(char *url);
    bool sendEnWordSearchRequest(char *word);
    bool sendEnWordSentenceRequest();
    static HttpManager* getInstance();
    QTimer* getRequestTimerRef();

    QNetworkReply* netPicReply;
    boost::shared_ptr<QNetworkReply> netWordReply;
    boost::shared_ptr<QNetworkReply> netSentenceReply;
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
    void slotSentenceReadyRead();
    void slotSentenceFinished();
    void slotError(enum QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError> list);

private:
    const int buffer_len;
    int buffer_pos;
    enData *model;
    static HttpManager* instance;
    static pthread_mutex_t mutex;
    QTimer requestTimer;
};

#endif // HTTPMANAGER_H
