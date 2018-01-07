#include "httpmanager.h"
#include <unistd.h>
#include "log.h"
#include "endata.h"
#include <errno.h>
#include <fcntl.h>

HttpManager::HttpManager(QObject *parent) : QObject(parent)
{
    netPicReply = NULL;
    manager = new QNetworkAccessManager(this);
}

void HttpManager::slotPicReadyRead()
{
    QByteArray bytes = netPicReply->readAll();
    QString url = netPicReply->request().url().toString();
    int fd;
    if(searchFilesMap(url.toStdString())){ //exist
        fd = filesMap[url.toStdString()];
        size_t ret = write(fd, bytes.toStdString().c_str(), bytes.size());
        if(ret <= 0){
            LOGDBG("write failed: %s", strerror(errno));
        }
    }
    else { //not exist
        string path = getIncomLocalPicPath(url.toStdString());
        fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
        if(fd == -1){
            LOGDBG("open failed, %s",strerror(  errno));
            return ;
        }
        size_t ret = write(fd, bytes.toStdString().c_str(), bytes.size());
        if(ret <= 0){
            LOGDBG("write failed: %s", strerror(errno));
        }
        filesMap[url.toStdString()] = fd;
    }
    LOGDBG("fd: %d, url: %s", fd, netPicReply->request().url().toString().toStdString().c_str());
}

void HttpManager::slotPicFinished()
{
    QString url = netPicReply->request().url().toString();
    QByteArray bytes = netPicReply->readAll();
    int fd = filesMap[url.toStdString()];
    close(fd);
    string f1;
    string f2;
    f1 = getIncomLocalPicPath(url.toStdString());
    f2 = getComLocalPicPath(url.toStdString());
    if(0 != rename(f1.c_str(),f2.c_str())){
        LOGDBG("rename failed: %s", strerror(errno));
    }
    LOGDBG("%d, finished.", fd);
}

bool HttpManager::searchFilesMap(string g_url)
{
    map<string, int>::iterator it;
    it = filesMap.find(g_url);
    if(it != filesMap.end()) return true;
    return false;
}

string HttpManager::getIncomLocalPicPath(string url)
{
    char* ptr = (char *)strrchr(url.c_str(),'/');
    if(NULL == ptr){
        LOGDBG("ptr is NULL for %s",url.c_str());
        return "";
    }
    string result;
    result = DOWNFILES_PICS_PATH;
    result = result+ptr;
    return result;
}

string HttpManager::getComLocalPicPath(string url)
{
    char* ptr = (char *)strrchr(url.c_str(),'/');
    if(NULL == ptr){
        LOGDBG("ptr is NULL for %s",url.c_str());
        return "";
    }
    string result;
    result = DOWNFILES_PICS_PATH;
    result = result+"/pic_";
    result = result+(ptr+1);
    return result;
}

bool HttpManager::sendPicRequest(char *url)
{
    QNetworkRequest *request = new QNetworkRequest ();
    request->setUrl(QUrl(url));
    request->setRawHeader("Authorization","Bearer mUIESkRq3QpCXolnS9eOyr5NlGJMul");
    netPicReply = manager->get(*request);
    if(NULL == netPicReply){
        return false;
    }
    connect(netPicReply, SIGNAL(readyRead()), this, SLOT(slotPicReadyRead()));
    connect(netPicReply, SIGNAL(error(QNetworkReply::NetworkError)),
          this, SLOT(slotError(QNetworkReply::NetworkError)));
    // SSL(Secure Sockets Layer 安全套接层), it encrypts data.
    connect(netPicReply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(netPicReply, SIGNAL(finished()), this, SLOT(slotPicFinished()));
    delete request;
    return true;
}

void HttpManager::slotError(enum QNetworkReply::NetworkError val)
{
    LOGDBG("error: %d",val);
}

void HttpManager::slotSslErrors(QList<QSslError> list)
{
    LOGDBG("%s","ssl errors:");
    foreach (QSslError error, list) {
        qDebug()<<error.errorString();
    }
}
