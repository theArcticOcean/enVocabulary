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
#include <sstream>
#include <fstream>
#include <algorithm>

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
    if(db.open()){
        query->prepare("CREATE TABLE IF NOT EXISTS Statement (sentences text PRIMARY KEY, translation text)");
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

    /*
    if(db.open()){
        query->prepare("CREATE TABLE IF NOT EXISTS Vocabulary (myWord text, translation text, secs INTEGER, PRIMARY KEY(myWord, secs))");
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
    */

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
/*
    if(0 != access(DOWNFILES_PICS_PATH, F_OK)){
#ifdef Q_OS_WIN32
        if(0 != mkdir(DOWNFILES_PICS_PATH)){
#else
        if(0 != mkdir(DOWNFILES_PICS_PATH, 0777)){
            LOGDBG("mkdir failed: %s", strerror(errno));
#endif
        }
    }
*/

    if(0 != access(DOWNFILES_WORDS_PATH, F_OK)){
#ifdef Q_OS_WIN32
        if(0 != mkdir(DOWNFILES_WORDS_PATH)){
#else
        if(0 != mkdir(DOWNFILES_WORDS_PATH, 0777)){
            LOGDBG("mkdir failed: %s", strerror(errno));
#endif
        }
    }

    //picsCheck();
    getCollectWordFromDisc();
    wordInf.init();
    memset(number,0,sizeof(number));
    num_size = 0;

#ifdef DEBUG
    showTableSentence();
    showTableVocabulary();
#endif
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
*           equal to MAX_PICS_NUMBER.
*           But I need not pictures in my software in the end for production, so the function is
*           not necessary.
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

bool enData::strIsNum(string str)
{
    double de;
    char ch;
    stringstream in(str);
    if(!(in>>de) || in>>ch){
        return false;
    }
    return true;
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
/*
* @brief    this function is used to show table Vocabulary's content
* @author   weiyang
* @date     2018.03.21
*/
void enData::showTableVocabulary()
{
    LOGDBG("start show collect words:");
    DIR *dir = opendir(DOWNFILES_WORDS_PATH);
    if(NULL == dir){
        LOGDBG("opendir failed: %s", strerror(errno));
        return ;
    }

    struct dirent *entry = readdir(dir);
    string buffer;
    buffer.reserve(1024);
    buffer = "";
    while(NULL != entry){
        if(entry->d_name[0] == '.'){
            entry = readdir(dir);
            continue;
        }
        if(!strIsNum(entry->d_name)){
            LOGDBG("this file's name is not a number.");
            entry = readdir(dir);
            continue;
        }
        QString path = DOWNFILES_WORDS_PATH;
        path = path+"/";
        path = path+entry->d_name;
        wordUnit myWord = readWordFile(path.toStdString().c_str());
        buffer = buffer + myWord.c_str();
        entry = readdir(dir);
    }
    write(STDOUT_FILENO, buffer.c_str(),buffer.length());
    LOGDBG("end!");
    //LOGDBG("bytes: %d\n%send!",buffer.length(),buffer.c_str());
}

void enData::showTableSentence()
{
    LOGDBG("start show table sentence:");
    if(db.open()) {
        query->prepare("select * from Statement");
        if(!query->exec()) {
            LOGDBG("query exec failed: %s", query->lastError().text().toStdString().c_str());
        }
        else {
            while(query->next()) {
                qDebug()<<query->value(0)<<" "<<query->value(1);
            }
        }
        db.close();
    }
    else {
        LOGDBG("open failed: %s",db.lastError().text().toStdString().c_str());
    }
}

void enData::wordInfShow()
{
    string str;
    char buff[64] = {0};
    str.reserve(1024);
    str = "";
    str += "cn_definition: ";       str += wordInf.cn_definition.toStdString();         str += "\n";
    str += "en_definition: ";       str += wordInf.en_definition.toStdString();         str += "\n";
    str += "uk_audio_addresses: ";  str += wordInf.uk_audio_addresses.toStdString();    str += "\n";
    str += "uk_pronunciation: ";    str += wordInf.uk_pronunciation.toStdString();      str += "\n";
    str += "us_audio_addresses: ";  str += wordInf.us_audio_addresses.toStdString();    str += "\n";
    str += "us_pronunciation: ";    str += wordInf.us_pronunciation.toStdString();      str +="\n";

    str += "vocabulary_id: ";
    sprintf(buff,"%.0lf\n",wordInf.vocabulary_id);
    str += buff;
    str += "\n";

    LOGDBG("\n%s",str.c_str());
}

void enData::sentencesShow()
{
    string str;
    str.reserve(1024);
    str = "";
    int len = v_sentences.size();
    for(int i=0; i<len; i++){
        sentenceUnit tmp = v_sentences[i];
        str += "sentence: ";
        str += tmp.sentence.toStdString();
        str += "\n";
        str += "translation: ";
        str += tmp.translation.toStdString();
        str += "\n\n";
    }
    LOGDBG("\n%s",str.c_str());
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

bool enData::addWordToDB(QString str)
{
    LOGDBG("start");
    wordUnit newWord(str.toStdString(), wordInf.cn_definition.toStdString());
    if(newWord.translation.length() <= 0){
        LOGDBG("word's translation is empty, so request again");
        return false;
    }

    // avoid inserting same word into database.
    if(checkWordInDB(str.toStdString())){
        LOGDBG("same word in disc.");
        map<ulong, wordUnit>::iterator it;
        for(it = m_collectWords.begin(); it != m_collectWords.end(); it++){
            if(newWord == it->second){
                uintegerToStr(it->first);
                m_collectWords.erase(it);
                string path = DOWNFILES_WORDS_PATH;
                path = path+"/";
                path = path+number;
                if(0 != unlink(path.c_str())){
                    LOGDBG("unlink failed: %s for %s",strerror(errno),number);
                    return false;
                }
                LOGDBG("rm same word for %s",path.c_str());
                break;
            }
        }
    }

    // insert word into database.
    unsigned long secs = time(NULL);
    m_collectWords[secs] = newWord;
    QString path = DOWNFILES_WORDS_PATH;
    path = path+"/";
    path = path + QString::number(secs);
    ofstream out(path.toStdString());
    out<<newWord;
    out.close();
    LOGDBG("add new Word %s in %s", newWord.word.c_str(), path.toStdString().c_str());

    // limit count in disc.
    if(!wordStoreLimit()){
        LOGDBG("wordStoreLimit failed.");
        return false;
    }
    LOGDBG("end!");
    return true;
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
            else {
                QString tmp = strList[0];
                LOGDBG("delete sentence for %s",tmp.toStdString().c_str());
            }
            db.close();
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
        db.close();
    }
    else {
        LOGDBG("open failed: %s",db.lastError().text().toStdString().c_str());
    }
    LOGDBG("end!");
    return ret;
}

bool enData::checkWordInDB(string str)
{
    LOGDBG("start");
    bool ret = false;
    wordUnit tmp(str, wordInf.cn_definition.toStdString());
    for(auto it: m_collectWords){
        if(tmp == it.second){
            ret = true;
            break;
        }
    }
    LOGDBG("end, and ret is %d", ret);
    return ret;
}

void enData::getCollectSentencePage(const int index)
{
    LOGDBG("start!");
    if(db.open()){
        query->prepare("select * from Statement limit :limit offset :offset");
        query->bindValue(":limit", COLLECT_SENTENCE_PAGESIZE);
        query->bindValue(":offset",(index-1)*COLLECT_SENTENCE_PAGESIZE);
        if(!query->exec()) {
            LOGDBG("query exec failed: %s", query->lastError().text().toStdString().c_str());
        }
        else {
            v_collectSentences.clear();
            while(query->next()) {
                sentenceUnit tmp;
                tmp.sentence = query->value("sentences").toString();
                tmp.translation = query->value("translation").toString();
                v_collectSentences.push_back(tmp);
            }
        }
        db.close();
    }
    else {
        LOGDBG("open failed: %s",db.lastError().text().toStdString().c_str());
    }
    LOGDBG("end!");
}

wordUnit enData::readWordFile(const char *path)
{
    LOGDBG("start, path is %s",path);
    ifstream in(path);
    if(!in.is_open()){
        LOGDBG("open failed for %s: %s", path, strerror(errno));
        return wordUnit();
    }
    wordUnit myWord;
    in>>myWord.word;
    in>>myWord.translation;
    while(!in.eof()){
        string str;
        in>>str;
        myWord.translation += " ";
        myWord.translation += str;
    }
    in.close();
    LOGDBG("end!");
    return myWord;
}
/*
* @brief    make sure the count of words is less than limit.
* @author   weiyang
* @date     2018.03.21
*/
bool enData::wordStoreLimit()
{
    LOGDBG("start");
    while(m_collectWords.size() > MAX_WORD_NUMBER){
        map<ulong, wordUnit>::iterator it;
        it = m_collectWords.end();
        it--;   //delete the futherest history record
        uintegerToStr(it->first);
        string path = DOWNFILES_WORDS_PATH;
        path = path+"/";
        path = path+number;
        if(0 != unlink(path.c_str())){
            LOGDBG("unlink failed: %s for %s",strerror(errno),number);
            return false;
        }
        LOGDBG("remove word: %s", path.c_str());
        m_collectWords.erase(it);
    }
    LOGDBG("end!");
    return true;
}

void enData::getCollectWordFromDisc()
{
    DIR *dir = opendir(DOWNFILES_WORDS_PATH);
    if(NULL == dir){
        LOGDBG("opendir failed: %s", strerror(errno));
        return ;
    }

    struct dirent *entry;
    entry = readdir(dir);
    m_collectWords.clear();

    while(NULL != entry){
        if(entry->d_name[0] == '.'){
            entry = readdir(dir);
            continue;
        }
        // it's decimal number which has 10 digits
        if(strlen(entry->d_name) <= 9 || !strIsNum(entry->d_name)){
            unlink(entry->d_name);   //delete incomplete files.
        }
        else {
            wordUnit tmp;
            string path;
            path = DOWNFILES_WORDS_PATH;
            path = path+"/";
            path = path+entry->d_name;
            tmp = readWordFile(path.c_str());
            if(tmp.word != ""){
                m_collectWords[stoul(entry->d_name)] = tmp;
            }
        }
        entry = readdir(dir);
    }
    wordStoreLimit();
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
        db.close();
    }
    else {
        LOGDBG("open failed: %s", db.lastError().text().toStdString().c_str());
    }
    return ret;
}

int enData::getColWordPageCount()
{
    int ret = 0;
    ret = (m_collectWords.size()+COLLECT_WORD_PAGESIZE-1)
            / COLLECT_WORD_PAGESIZE;
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

/*
* @brief    the function is used to convert unsigned integer to string.
* @author   weiyang
* @date     2018.03.21
*/
template<typename T>
void enData::uintegerToStr(T value)
{
    memset(number,0,sizeof(number));
    num_size = 0;
    while(value){
        short b = value%10;
        value = value/10;
        number[num_size++] = b+'0';
    }
    for(int i=0; i<num_size/2; i++){
        std::swap(number[i], number[num_size-1-i]);
    }
}
