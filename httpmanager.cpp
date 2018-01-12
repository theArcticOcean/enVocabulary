#include "httpmanager.h"
#include <unistd.h>
#include "log.h"
#include "endata.h"
#include <errno.h>
#include <fcntl.h>
#include <QJsonDocument>
#include <QJsonObject>

HttpManager::HttpManager(QObject *parent) :
    QObject(parent),
    buffer_len(1024)
{
    netPicReply = NULL;
    manager = new QNetworkAccessManager(this);
    buffer = (char *)malloc(buffer_len);
    memset(buffer, 0, buffer_len);
    buffer_pos = 0;
    model = enData::getInstance();
}

HttpManager::~HttpManager()
{
    delete netPicReply;
    delete netWordReply;
    delete manager;
    free(buffer);
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

void HttpManager::slotEnWordReadyRead()
{
    QByteArray bytes;
    bytes = netWordReply->readAll();
    buffer_pos += sprintf(buffer+buffer_pos, bytes.toStdString().c_str());
}

void HttpManager::slotEnWordFinished()
{
    QByteArray bytes;
    QJsonDocument jsonDoc;
    QJsonObject json;
    QJsonParseError error;

    if(strlen(buffer) <= 0){
        LOGDBG("buffer has no data.");
        return ;
    }
    jsonDoc = QJsonDocument::fromJson(buffer, &error);
    bytes = jsonDoc.toJson();
    if(bytes.isEmpty()) {
        LOGDBG("bytes is empty: %s",error.errorString().toStdString().c_str());
        return;
    }
    LOGDBG("json:\n%s", bytes.toStdString().c_str());
    json = jsonDoc.object();
    model->jsonParseForWord(json);
    buffer_pos = 0;
    memset(buffer, 0, strlen(buffer));
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
    string str = "Bearer ";
    str = str+ACCESS_TOKEN;
    request->setRawHeader("Authorization",str.c_str());
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

bool HttpManager::sendEnWordSearch(char *word)
{
    QNetworkRequest *request = new QNetworkRequest ();
    string url = ENWORD_SEARCH_ENTRY;
    url = url+"?word=";
    url = url+word;
    url = url+"&access_token=";
    url = url+ACCESS_TOKEN;
    request->setUrl(QUrl(url.c_str()));
    LOGDBG("url:  %s", request->url().toString().toStdString().c_str());
    string str = "Bearer ";
    str = str+ACCESS_TOKEN;
    request->setRawHeader("Authorization",str.c_str());
    netWordReply = manager->get(*request);
    if(NULL == netWordReply){
        return false;
    }
    connect(netWordReply, SIGNAL(readyRead()), this, SLOT(slotEnWordReadyRead()));
    connect(netWordReply, SIGNAL(error(QNetworkReply::NetworkError)),
          this, SLOT(slotError(QNetworkReply::NetworkError)));
    // SSL(Secure Sockets Layer 安全套接层), it encrypts data.
    connect(netWordReply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(netWordReply, SIGNAL(finished()),
            this, SLOT(slotEnWordFinished()));
    delete request;
    LOGDBG("end!");
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
