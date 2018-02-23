#include "endata.h"
#include "log.h"
#include <queue>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <QJsonParseError>
#include <QJsonArray>
#include "controller.h"
#include "endata.h"
#include <QCoreApplication>
#include <QApplication>

#ifdef Q_OS_WIN32
#include <io.h>
#endif

using namespace std;

pthread_mutex_t enData::instanceMutex = PTHREAD_MUTEX_INITIALIZER;
enData* enData::instance = NULL;

enData::enData()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString fileName = QCoreApplication::applicationDirPath()+"/.enVocabDataBase";
    db.setDatabaseName(fileName);
    query = new QSqlQuery(db);
    if(-1 == access(fileName.toStdString().c_str(), F_OK)){      //the file doesn't exsit.
        if(db.open()){
            query->prepare("create table Statement (sentences text primary key, translation text)");
            if(!query->exec()) {
                LOGDBG("create: %s", query->lastError().text().toStdString().c_str());
            }
            else {
                LOGDBG("create table ok!");
            }
            db.close();
        }
        else {
            LOGDBG("create table failed: %s\n"
                   "path is %s",
                   db.lastError().text().toStdString().c_str(),
                   db.databaseName().toStdString().c_str());
        }
    }
    else {
        LOGDBG("%s had exsit",fileName.toStdString().c_str());
    }

    pthread_mutex_init(&instanceMutex, NULL);
    v_sentences.clear();

    if(0 != access(DOWNFILES_PATH, F_OK)){
#ifdef Q_OS_WIN32
        if(0 != mkdir(DOWNFILES_PATH)){
#else
        if(0 != mkdir(DOWNFILES_PATH, 0777)){
            LOGDBG("mkdir failed: %s", strerror(errno));
#endif
        }
    }
    if(0 != access(DOWNFILES_PICS_PATH, F_OK)){
#ifdef Q_OS_WIN32
        if(0 != mkdir(DOWNFILES_PICS_PATH)){
#else
        if(0 != mkdir(DOWNFILES_PICS_PATH, 0777)){
            LOGDBG("mkdir failed: %s", strerror(errno));
#endif
        }
    }
    picsCheck();
    wordInf.init();
}

enData::~enData()
{
    if(instance){
        delete instance;
        instance = NULL;
    }
    if(query){
        delete query;
        query = NULL;
    }
}

/*
* @brief    it works at program start, check the number of pictures, make sure it is less than
*           equal to MAX_PICS_NUMBER
* @author   weiyang
* @date     2017.12.31
*/
void enData::picsCheck()
{
    DIR *dir = opendir(DOWNFILES_PICS_PATH);
    if(NULL == dir){
        LOGDBG("opendir failed: %s", strerror(errno));
        return ;
    }
    struct dirent *entry;
    int number;
    queue<char *> files;         // front === back

    number = 0;
    entry = readdir(dir);
    while(NULL != entry){
        if(entry->d_name[0] == '.'){
            entry = readdir(dir);
            continue;
        }
        if(strlen(entry->d_name) > 4 && 0 != strncmp(entry->d_name,"pic_",4)){
            unlink(entry->d_name);   //delete incomplete files.
        }
        number++;
        files.push(entry->d_name);
        entry = readdir(dir);
    }

    while(number > MAX_PICS_NUMBER){
        unlink(files.front());
        files.pop();
        number--;
    }
}

enData *enData::getInstance()
{
    if(NULL == instance){
        pthread_mutex_lock(&instanceMutex);
        instance = new enData();
        pthread_mutex_unlock(&instanceMutex);
    }
    return instance;
}

void enData::jsonParseForWord(const QJsonObject cjson)
{
    QJsonValue value;
    QJsonParseError error;
    QJsonObject json = cjson;
    int varInt;

    if(json.contains("status_code")){
        varInt = json["status_code"].toInt();
        if(0 != varInt){
            LOGDBG("status_code: ", varInt);
            LOGDBG("msg: %s", json["msg"].toString().toStdString().c_str());
            return ;
        }
    }
    QJsonObject jsonTmp;
    if(json.contains("data")){
        jsonTmp = json["data"].toObject();
        if(!checkElementInJson(jsonTmp, "audio_addresses")) return;
        value = jsonTmp["audio_addresses"];
        jsonTmp = value.toObject();
        if(!checkElementInJson(jsonTmp, "uk")) return;
        QJsonArray audioArray = jsonTmp["uk"].toArray();
        wordInf.uk_audio_addresses = audioArray[0].toString();
        audioArray = jsonTmp["us"].toArray();
        wordInf.us_audio_addresses = audioArray[0].toString();

        jsonTmp = json["data"].toObject();
        if(!checkElementInJson(jsonTmp, "cn_definition")) return;
        value = jsonTmp["cn_definition"];
        jsonTmp = value.toObject();
        if(!checkElementInJson(jsonTmp, "defn")) return;
        wordInf.cn_definition = jsonTmp["defn"].toString();
        jsonTmp = json["data"].toObject();
        if(!checkElementInJson(jsonTmp, "en_definition")) return;
        value = jsonTmp["en_definition"];
        jsonTmp = value.toObject();
        if(!checkElementInJson(jsonTmp, "defn")) return;
        wordInf.en_definition = jsonTmp["defn"].toString();

        jsonTmp = json["data"].toObject();
        if(!checkElementInJson(jsonTmp, "pronunciations")) return;
        jsonTmp = jsonTmp["pronunciations"].toObject();
        if(!checkElementInJson(jsonTmp, "uk")) return;
        wordInf.uk_pronunciation = jsonTmp["uk"].toString();
        if(!checkElementInJson(jsonTmp, "us")) return;
        wordInf.us_pronunciation = jsonTmp["us"].toString();

        jsonTmp = json["data"].toObject();
        if(!checkElementInJson(jsonTmp, "id")) return;
        wordInf.vocabulary_id = jsonTmp["id"].toDouble();

        //wordInfShow();
        Controller *control = Controller::getInstance();
        control->sendViewMsg(SearchWordEnum);
    }
}

void enData::jsonParseForSentence(const QJsonObject cjson)
{
    QJsonValue value;
    QJsonParseError error;
    QJsonObject json = cjson;
    int varInt;

    if(json.contains("status_code")){
        varInt = json["status_code"].toInt();
        if(0 != varInt){
            LOGDBG("status_code: ", varInt);
            LOGDBG("msg: %s", json["msg"].toString().toStdString().c_str());
            return ;
        }
    }
    QJsonObject jsonTmp;
    if(json.contains("data")){
        QJsonArray sentencesJson= json["data"].toArray();
        int len = sentencesJson.size();
        v_sentences.clear();
        for(int i=0; i<len; i++){
            jsonTmp = sentencesJson[i].toObject();
            sentenceUnit senUnit;
            if(!jsonTmp.contains("translation")) continue;
            if(!jsonTmp.contains("annotation")) continue;
            senUnit.translation = jsonTmp["translation"].toString();
            senUnit.sentence = jsonTmp["annotation"].toString();
            v_sentences.push_back(senUnit);
            if(v_sentences.size() == SENTENCE_NUM) break;
        }
        sentencesShow();
        Controller *control = Controller::getInstance();
        control->sendViewMsg(GotoWordSentences);
    }
}

bool enData::checkElementInJson(QJsonObject &json, const string key)
{
    if(!json.contains(key.c_str())){
        string str = "there is no ";
        str = str+key;
        LOGDBG("%s", str.c_str());
        return false;
    }
    return true;
}

void enData::wordInfShow()
{
    char buff[1024] = {0};
    int ret = 0;
    ret += sprintf(buff+ret,"%s","cn_definition: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.cn_definition.toStdString().c_str());
    ret += sprintf(buff+ret,"%s","en_definition: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.en_definition.toStdString().c_str());
    ret += sprintf(buff+ret,"%s","uk_audio_addresses: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.uk_audio_addresses.toStdString().c_str());
    ret += sprintf(buff+ret,"%s","uk_pronunciation: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.uk_pronunciation.toStdString().c_str());
    ret += sprintf(buff+ret,"%s","us_audio_addresses: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.us_audio_addresses.toStdString().c_str());
    ret += sprintf(buff+ret,"%s","us_pronunciation: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.us_pronunciation.toStdString().c_str());
    ret += sprintf(buff+ret,"%s","vocabulary_id: ");
    ret += sprintf(buff+ret,"%.0lf\n",wordInf.vocabulary_id);

    LOGDBG("\n%s",buff);
}

void enData::sentencesShow()
{
    char buff[1024] = {0};
    int ret = 0;
    int len = v_sentences.size();
    for(int i=0; i<len; i++){
        sentenceUnit tmp = v_sentences[i];
        ret += sprintf(buff+ret,"%s","sentence: ");
        ret += sprintf(buff+ret,"%s\n",tmp.sentence.toStdString().c_str());
        ret += sprintf(buff+ret,"%s","translation: ");
        ret += sprintf(buff+ret,"%s\n\n",tmp.translation.toStdString().c_str());
    }
    LOGDBG("\n%s",buff);
}

void enData::addSentenceToDB(const int index)
{
    sentenceUnit tmp = v_sentences[index];
    if(db.open()){
        query->prepare("INSERT INTO Statement VALUES (:sentence, :translation)");
        query->bindValue(":sentence", simpleSentence(tmp.sentence));
        query->bindValue(":translation",simpleSentence(tmp.translation));

        if(!query->exec()){
            LOGDBG("query exec failed: %s", query->lastError().text().toStdString().c_str());
        }
        else {
            LOGDBG("insert sentence %d ok!", index);
        }
        db.close();
    }
    else {
        LOGDBG("open failed: %s",db.lastError().text().toStdString().c_str());
    }
}

void enData::deleteSentenceFromDB(const int index)
{
    LOGDBG("start");
    sentenceUnit tmp = v_sentences[index];
    if(db.open()){
        query->prepare("DELETE FROM Statement where sentences = :sentence ");
        query->bindValue(":sentence", simpleSentence(tmp.sentence));
        if(!query->exec()) {
            LOGDBG("query exec failed: %s", query->lastError().text().toStdString().c_str());
        }
        else {
            LOGDBG("delete sentence %d from DB ok!", index);
        }
        db.close();
    }
    else {
        LOGDBG("open failed: %s",db.lastError().text().toStdString().c_str());
    }
    LOGDBG("end!");
}

void enData::deleteSentenceFromDB(const QString text)
{
    QStringList strList = text.split("\n\n");
    if(strList.length()){
        if(db.open()){
            query->prepare("DELETE FROM Statement where sentences = :sentence ");
            query->bindValue(":sentence", strList[0]);
            if(!query->exec()){
                LOGDBG("delete failed: %s",query->lastError().text().toStdString().c_str());
            }
        }
        else {
            LOGDBG("db open failed: %s",db.lastError().text().toStdString().c_str());
        }
    }
}

bool enData::checkSentenceInDB(const int index)
{
    LOGDBG("start");
    bool ret = false;
    if(db.open()){
        sentenceUnit tmp = v_sentences[index];
        query->prepare("select count(*) from Statement where sentences = :sentence");
        query->bindValue(":sentence", simpleSentence(tmp.sentence));
        if(!query->exec()) {
            LOGDBG("query exec failed: %s", query->lastError().text().toStdString().c_str());
        }
        else {
            if(query->next()) {
                if(query->value(0).toInt() > 0){
                    ret = true;
                }
            }
        }
    }
    else {
        LOGDBG("open failed: %s",db.lastError().text().toStdString().c_str());
    }
    LOGDBG("end!");
    return ret;
}

void enData::getCollectSentencePage(const int index)
{
    if(db.open()){
        query->prepare("select * from Statement limit :limit offset :index");
        query->bindValue(":limit",6);
        query->bindValue(":index",index);
        if(!query->exec()) {
            LOGDBG("exec failed: %s",query->lastError().text().toStdString().c_str());
        }
        else {
            v_collectSentences.clear();
            while(query->next()){
                QString sentence = query->value("sentences").toString();
                QString translation = query->value("translation").toString();
                v_collectSentences.push_back(sentenceUnit(sentence,translation));
            }
        }
    }
    else {
        LOGDBG("open failed: %s", db.lastError().text().toStdString().c_str());
    }
}

int enData::getColSentencePageCount()
{
    int ret = 0;
    if(db.open()){
        query->prepare("select count(*) from Statement");
        if(!query->exec()) {
            LOGDBG("exec failed: %s",query->lastError().text().toStdString().c_str());
        }
        else {
            if(query->next()) {
                // ceiling operaton
                ret = (query->value(0).toInt()+COLLECT_SENTENCE_PAGESIZE-1)
                        / COLLECT_SENTENCE_PAGESIZE;
            } else{
                LOGDBG("no next");
            }
        }
    }
    else {
        LOGDBG("open failed: %s", db.lastError().text().toStdString().c_str());
    }
    return ret;
}

QString enData::simpleSentence(const QString sentence)
{
    QString tmp = sentence;
    tmp.remove("<b>");
    tmp.remove("</b>");
    tmp.remove("<vocab>");
    tmp.remove("</vocab>");
    return tmp;
}

int enData::getSentenceCount() const
{
    return v_sentences.size();
}
