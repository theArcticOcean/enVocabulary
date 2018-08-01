/**********************************************************
*
* @brief    HttpManager is responsible to request word's
*           information based on shanbay API.
*
* @author   theArcticOcean
***********************************************************/

#include "httpmanager.h"
#include <unistd.h>
#include <math.h>
#include "log.h"
#include "endata.h"
#include "controller.h"
#include <errno.h>
#include <fcntl.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

HttpManager* HttpManager::instance = NULL;
pthread_mutex_t HttpManager::mutex = PTHREAD_MUTEX_INITIALIZER;

/*
*   Constructor of class HttpManager.
*/
HttpManager::HttpManager(QObject *parent) :
    QObject(parent),
    buffer_len(BUFFER_LEN)
{
    netPicReply = NULL;
    netWordReply = NULL;
    manager = new QNetworkAccessManager(this);
    buffer = (char *)malloc(buffer_len);
    memset(buffer, 0, buffer_len);
    buffer_pos = 0;
    model = enData::getInstance();
}

/*
*   Destructor of class HttpManager.
*/
HttpManager::~HttpManager()
{
    if(NULL != netPicReply){
        delete netPicReply;
        netPicReply = NULL;
    }
    if(NULL != manager){
        delete manager;
        manager = NULL;
    }
    if(NULL != buffer){
        free(buffer);
        buffer = NULL;
    }
    if(NULL != instance){
        delete instance;
        instance = NULL;
    }
}

/*
*   The function is used to breakpoint resuming for picture receiving.
*/
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
#ifndef  Q_OS_WIN
        fd = open(path.c_str(),O_WRONLY | O_CREAT | O_TRUNC, 0755);
#else
        fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC);
#endif
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

/*
*   The function is used to finish receiving picture.
*/
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

/*
*   Slot function for QNetworkReply::readyRead
*/
void HttpManager::slotEnWordReadyRead()
{
    QByteArray bytes;
    bytes = netWordReply->readAll();
    buffer_pos += sprintf(buffer+buffer_pos, "%s", bytes.toStdString().c_str());
}

/*
*   After fetch all data from internet, analyze json data.
*/
void HttpManager::slotEnWordFinished()
{
    QByteArray bytes;
    QJsonDocument jsonDoc;
    QJsonObject json;
    QJsonParseError error;

    if(strlen(buffer) <= 0) {
        LOGDBG("buffer has no data.");
        return ;
    }
    jsonDoc = QJsonDocument::fromJson(buffer, &error);
    bytes = jsonDoc.toJson();
    if(bytes.isEmpty()) {
        LOGDBG("bytes is empty: %s",error.errorString().toStdString().c_str());
        return;
    }
    //LOGDBG("json:\n%s", bytes.toStdString().c_str());
    json = jsonDoc.object();
    model->jsonParseForWord(json);
    memset(buffer, 0, buffer_pos);
    buffer_pos = 0;
}

/*
*   Slot function for request word's sentences.
*/
void HttpManager::slotSentenceReadyRead()
{
    QByteArray bytes;
    bytes = netSentenceReply->readAll();
    buffer_pos += sprintf(buffer+buffer_pos, "%s", bytes.toStdString().c_str());
}

/*
*   After fetch all data from internet, analyze json data.
*/
void HttpManager::slotSentenceFinished()
{
    QByteArray bytes;
    QJsonDocument jsonDoc;
    QJsonObject json;
    QJsonParseError error;

    if(strlen(buffer) <= 0) {
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
    model->jsonParseForSentence(json);
    buffer_pos = 0;
    memset(buffer, 0, strlen(buffer));
}

/*
*   Check whether picture file exists.
*/
bool HttpManager::searchFilesMap(string g_url)
{
    map<string, int>::iterator it;
    it = filesMap.find(g_url);
    if(it != filesMap.end()) return true;
    return false;
}

/*
*   Get the name of incomplete local file for storing picture.
*/
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

/*
*   Get the name of complete local file for storing picture.
*/
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

/*
*   Interface for requesting picture.
*/
bool HttpManager::sendPicRequest(char *url)
{
    if(NULL != netPicReply){
        disconnect(netPicReply, SIGNAL(readyRead()), this, SLOT(slotPicReadyRead()));
        disconnect(netPicReply, SIGNAL(error(QNetworkReply::NetworkError)),
              this, SLOT(slotError(QNetworkReply::NetworkError)));
        disconnect(netPicReply, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(slotSslErrors(QList<QSslError>)));
        disconnect(netPicReply, SIGNAL(finished()), this, SLOT(slotPicFinished()));
    }
    QNetworkRequest *request = new QNetworkRequest ();
    if(NULL == request){
        LOGDBG("request is NULL");
        return false;
    }
    string str = url;
    str = str+"&";
    str = str+TOKEN_STR;
    request->setUrl(QUrl(str.c_str()));
    netPicReply = manager->get(*request);
    if(NULL == netPicReply){
        LOGDBG("netPicReply is NULL");
        delete request;
        return false;
    }
    connect(netPicReply, SIGNAL(readyRead()), this, SLOT(slotPicReadyRead()));
    connect(netPicReply, SIGNAL(error(QNetworkReply::NetworkError)),
          this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(netPicReply, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(netPicReply, SIGNAL(finished()), this, SLOT(slotPicFinished()));
    delete request;
    return true;
}

/*
*   Interface for request word information.
*/
bool HttpManager::sendEnWordSearchRequest(char *word)
{
    if(NULL != netWordReply){
        disconnect(netWordReply.get(), SIGNAL(readyRead()), this, SLOT(slotEnWordReadyRead()));
        disconnect(netWordReply.get(), SIGNAL(error(QNetworkReply::NetworkError)),
              this, SLOT(slotError(QNetworkReply::NetworkError)));
        // SSL(Secure Sockets Layer 安全套接层), it encrypts data.
        disconnect(netWordReply.get(), SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(slotSslErrors(QList<QSslError>)));
        disconnect(netWordReply.get(), SIGNAL(finished()), this, SLOT(slotEnWordFinished()));
    }
    QNetworkRequest *request = new QNetworkRequest ();
    if(NULL == request){
        LOGDBG("request is NULL");
        return false;
    }
    string url = ENWORD_SEARCH_ENTRY;
    url = url+"?word=";
    url = url+word;
    url = url+"&";
    url = url+TOKEN_STR;
    request->setUrl(QUrl(url.c_str()));
    LOGDBG("url:  %s", request->url().toString().toStdString().c_str());
    netWordReply.reset(manager->get(*request));
    if(NULL == netWordReply){
        LOGDBG("netWordReply is NULL");
        delete request;
        return false;
    }
    connect(netWordReply.get(), SIGNAL(readyRead()), this, SLOT(slotEnWordReadyRead()));
    connect(netWordReply.get(), SIGNAL(error(QNetworkReply::NetworkError)),
          this, SLOT(slotError(QNetworkReply::NetworkError)));
    // SSL(Secure Sockets Layer 安全套接层), it encrypts data.
    connect(netWordReply.get(), SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(netWordReply.get(), SIGNAL(finished()), this, SLOT(slotEnWordFinished()));
    delete request;
    LOGDBG("end!");
    return true;
}

/*
*   Interface for requesting word's sentences.
*/
bool HttpManager::sendEnWordSentenceRequest()
{
    LOGDBG("start");
    memset(buffer, 0, buffer_len);
    buffer_pos = 0;
    if(NULL != netSentenceReply.get()){
        disconnect(netSentenceReply.get(), SIGNAL(readyRead()), this, SLOT(slotSentenceReadyRead()));
        disconnect(netSentenceReply.get(), SIGNAL(error(QNetworkReply::NetworkError)),
              this, SLOT(slotError(QNetworkReply::NetworkError)));
        disconnect(netSentenceReply.get(), SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(slotSslErrors(QList<QSslError>)));
        disconnect(netSentenceReply.get(), SIGNAL(finished()), this, SLOT(slotSentenceFinished()));
    }

    boost::shared_ptr<QNetworkRequest> request(new QNetworkRequest());
    if(NULL == request) {
        LOGDBG("request is NULL");
        return false;
    }
    if(abs(model->wordInf.vocabulary_id) < NUM_ERROR){
        LOGDBG("the vocabulary_id is less than 1.");
        return false;
    }
    string url = SENTENCE_SEARCH_ENTRY;
    url = url+"?vocabulary_id=";
    char id[32] = {0};
    sprintf(id, "%.0lf", model->wordInf.vocabulary_id);
    url = url+id;
    url = url+"&";
    url = url+TOKEN_STR;
    request.get()->setUrl(QUrl(url.c_str()));
    LOGDBG("url:  %s", request.get()->url().toString().toStdString().c_str());
    //netSentenceReply = boost::make_shared(manager->get(*request.get()));
    netSentenceReply.reset(manager->get(*request.get()));
    if(NULL == netSentenceReply.get()){
        LOGDBG("netSentenceReply is NULL");
        return false;
    }
    connect(netSentenceReply.get(), SIGNAL(readyRead()), this, SLOT(slotSentenceReadyRead()));
    connect(netSentenceReply.get(), SIGNAL(error(QNetworkReply::NetworkError)),
          this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(netSentenceReply.get(), SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
    connect(netSentenceReply.get(), SIGNAL(finished()), this, SLOT(slotSentenceFinished()));
    LOGDBG("end!");
    return true;
}

/*
*   Get singleton pointer which point to HttpManager object.
*   In fact, our software has not multi-thread mechanism, it's just for practice.
*/
HttpManager *HttpManager::getInstance()
{
    if(NULL == instance){
        pthread_mutex_lock(&mutex);
        instance = new HttpManager();
        pthread_mutex_unlock(&mutex);
    }
    return instance;
}

/*
*   Handle reply error of internet request.
*/
void HttpManager::slotError(enum QNetworkReply::NetworkError val)
{
    if( QNetworkReply::AuthenticationRequiredError == val )
    {
        Controller::getInstance()->sendViewMsg(AccessTokenInvalid);
    }
    LOGDBG("error: %d",val);
}

/*
*   Handle ssl error of internet request.
*/
void HttpManager::slotSslErrors(QList<QSslError> list)
{
    LOGDBG("%s","ssl errors: ");
    QNetworkReply *reply = dynamic_cast<QNetworkReply*>(sender());
    foreach (QSslError error, list) {
        qDebug()<<error.errorString();
    }
}
