/**********************************************************
*
* @brief    Class enData is used to store vocabulary data,
*           delete local data and load local data at start-up
*           time.
*
* @author   theArcticOcean
***********************************************************/

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

/*
*   Constructor of class enData. Create database, table and local files
*   if they don't exist.
*/
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

    if(0 != access(DOWNFILES_WORDS_PATH, F_OK)){
#ifdef Q_OS_WIN32
        if(0 != mkdir(DOWNFILES_WORDS_PATH)){
#else
        if(0 != mkdir(DOWNFILES_WORDS_PATH, 0777)){
            LOGDBG("mkdir failed: %s", strerror(errno));
#endif
        }
    }

    getCollectWordFromDisc();
    wordInf.init();
    memset(number,0,sizeof(number));
    num_size = 0;

#ifdef DEBUG
    showTableSentence();
    showTableVocabulary();
#endif
}

/*
*   Destructor of class enData
*/
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
*   The function works at program start, check the number of pictures, make sure it is less than
*   equal to MAX_PICS_NUMBER.
*   But I need not pictures in my software in the end for production, so the function is not
*   necessary.
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

/*
*   Get singleton pointer for enData.
*   In fact, our software has not multi-threads mechanism, the mutex added is just for practice.
*/
enData *enData::getInstance()
{
    if(NULL == instance){
        pthread_mutex_lock(&instanceMutex);
        instance = new enData();
        pthread_mutex_unlock(&instanceMutex);
    }
    return instance;
}

/*
*   Judge whether string contains number.
*/
bool enData::strIsNum(string str)
{
    double de;
    char ch;
    stringstream in(str);
    // The following code is what I read on the internet, actually I think !(in>>de) is enough.
    if( !(in>>de) || in>>ch )
    {
        return false;
    }
    return true;
}

/*
*   Analyzes json data from shanbay and stores information, then notifies UIMgr to update.
*/
void enData::jsonParseForWord(const QJsonObject &cjson)
{
    QJsonValue value;
    QJsonObject json = cjson;
    int varInt;

    if(json.contains("status_code")){
        varInt = json["status_code"].toInt();
        if(0 != varInt){
            LOGDBG("status_code: %d", varInt);
            LOGDBG("msg: %s", json["msg"].toString().toStdString().c_str());
            Controller::getInstance()->sendViewMsg(WordNotFound);
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
#ifdef DEBUG
        wordInfShow();
#endif
        Controller *control = Controller::getInstance();
        control->sendViewMsg(SearchWord);
    }
}

/*
*   Analyzes json data from shanbay and stores information, then notifies UIMgr to update.
*/
void enData::jsonParseForSentence(const QJsonObject &cjson)
{
    QJsonValue value;
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
            senUnit.translation = jsonTmp["translation"].toString(); //simpleSentence
            senUnit.sentence = jsonTmp["annotation"].toString();
            v_sentences.push_back(senUnit);
            if(v_sentences.size() == SENTENCE_NUM) break;
        }
#ifdef DEBUG
        sentencesShow();
#endif
        Controller *control = Controller::getInstance();
        control->sendViewMsg(GotoWordSentences);
    }
}

/*
*   Simple interface to check whether json data contains special key.
*/
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
*   This function is used to show table Vocabulary's content.
*   It's always used in debug scene.
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

/*
*   This function is used to show all data of table Statement.
*   It's always used in debug scene.
*/
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

/*
*   The function is used to show word information in debug scene.
*/
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

/*
*   The function is used to show sentence information in debug scene.
*/
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

/*
*   Insert sentence into Sqlite database.
*/
void enData::addSentenceToDB(const int index)
{
    sentenceUnit tmp = v_sentences[index];
    if(db.open()){
        query->prepare("INSERT INTO Statement VALUES (:sentence, :translation)");
        query->bindValue(":sentence",  tmp.sentence); //simpleSentence
        query->bindValue(":translation", tmp.translation);

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

/*
*   Store word and save as local file just like database.
*/
bool enData::addWordToDB(QString str)
{
    LOGDBG("start");
    string definition = "";
    QStringList list = wordInf.cn_definition.split("\n");
    for(QString it: list){
        definition = definition + it.toStdString();
        definition += " ";
    }
    definition = definition.substr(0, definition.length()-1);
    wordUnit newWord(str.toStdString(), definition);

    if(newWord.translation.length() <= 0){
        LOGDBG("word's translation is empty, so request again");
        return false;
    }

    //! avoid inserting same word into database.
    if(checkWordInDB(str.toStdString())){
        LOGDBG("same word in disc.");
        map<ulong, wordUnit>::iterator it;
        for(it = m_collectWords.begin(); it != m_collectWords.end();){
            if(newWord.word == it->second.word){
                uintegerToStr(it->first);
                //! notice: use it++ can erase it and move iterator to next.
                m_collectWords.erase(it++);
                string path = DOWNFILES_WORDS_PATH;
                path = path+"/";
                path = path+number;
                if(0 != unlink(path.c_str())){
                    LOGDBG("unlink failed: %s for %s",strerror(errno),number);
                    return false;
                }
                LOGDBG("rm same word for %s",path.c_str());
            }
            else it++;
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

/*
*   Delete special recoid from database table by index for mySentences window.
*/
void enData::deleteSentenceFromDBOnMySentencesWnd(const int index)
{
    LOGDBG("start");
    sentenceUnit tmp = v_sentences[index];
    if(db.open()){
        query->prepare("DELETE FROM Statement where sentences = :sentence ");
        query->bindValue(":sentence", tmp.sentence);
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

/*
*   Delete special recoid from database table by index for collectedSentences window.
*/
void enData::deleteSentenceFromDBOnCollectedSentencesWnd(const int index)
{
    LOGDBG("start");
    sentenceUnit tmp = v_collectSentences[index];
    if(db.open()){
        query->prepare("DELETE FROM Statement where sentences = :sentence ");
        query->bindValue(":sentence", tmp.sentence);
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

/*
*   Delete all collected sentences from sqlite database.
*/
void enData::clearSentenceFromDB()
{
    if(db.open()){
        query->prepare("DELETE FROM Statement");
        if(!query->exec()) {
            LOGDBG("delete failed: %s",query->lastError().text().toStdString().c_str());
        }
        else {
            LOGDBG("clear statement table in disc");
        }
        db.close();
    }
    else {
        LOGDBG("db open failed: %s",db.lastError().text().toStdString().c_str());
    }
}

/*
*   Delete all collected sentences form memory.
*   Notice: our software reads data from disc when gets new page.
*/
void enData::clearSentenceInMem()
{
    v_collectSentences.clear();
}

/*
*   Delete all word files from disc.
*/
void enData::clearWordFromDisc()
{
    LOGDBG("start");
    DIR *dir = opendir(DOWNFILES_WORDS_PATH);
    if(NULL == dir){
        LOGDBG("opendir failed: %s",strerror(errno));
        return ;
    }
    struct dirent *entry = readdir(dir);
    while(NULL != entry){
        if(entry->d_name[0] == '.'){
            entry = readdir(dir);
            continue;
        }
        string path = DOWNFILES_WORDS_PATH;
        path = path+"/";
        path = path+entry->d_name;
        if(-1 == unlink(path.c_str())){
            LOGDBG("unlink failed for %s: %s",path.c_str(),strerror(errno));
        }
        entry = readdir(dir);
    }
    LOGDBG("end!");
}

/*
*   Delete all collected words in memory.
*/
void enData::clearWordInMem()
{
    m_collectWords.clear();
}

/*
*   Check whether special sentence exists in database according to index.
*/
bool enData::checkSentenceInDB(const int index)
{
    LOGDBG("start");
    bool ret = false;
    if(db.open()){
        sentenceUnit tmp = v_sentences[index];
        query->prepare("select count(*) from Statement where sentences = :sentence");
        query->bindValue(":sentence", tmp.sentence);
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

/*
*   Check whether collected words is in memory, m_collectWords would be wrote into disc at close
*   time.
*/
bool enData::checkWordInDB(string str)
{
    LOGDBG("start");
    bool ret = false;
    wordUnit tmp(str, wordInf.cn_definition.toStdString());
    for(auto it: m_collectWords){
        if(tmp.word == it.second.word){
            ret = true;
            break;
        }
    }
    LOGDBG("end, and ret is %d", ret);
    return ret;
}

/*
*   Fetch collecte sentences in special page from disc.
*/
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

/*
*   Get word information from local word file.
*/
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
*   The function is used to make sure the count of words is less than limit.
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

/*
*   Get collected word information from local word file.
*/
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

/*
*   Get page count of collected sentences.
*/
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

/*
*   Get page count of collected word.
*/
int enData::getColWordPageCount()
{
    int ret = 0;
    ret = (m_collectWords.size()+COLLECT_WORD_PAGESIZE-1)
            / COLLECT_WORD_PAGESIZE;
    return ret;
}

/*
*   Remove html tag from internet data.
*/
QString enData::simpleSentence(const QString sentence)
{
    QString tmp = sentence;
    tmp.remove("<b>");
    tmp.remove("</b>");
    tmp.remove("<vocab>");
    tmp.remove("</vocab>");

    tmp.replace('.',".\n");
    tmp.replace("。","。\n");
    tmp.replace('?',"?\n");
    tmp.replace("？","？\n");
    tmp.replace('!',"!\n");
    tmp.replace("！","！\n");
    return tmp;
}

/*
*   Get the count of word's example sentences.
*/
int enData::getSentenceCount() const
{
    return v_sentences.size();
}

/*
*   The function is used to convert integer to string.
*/
template<typename T>
void enData::uintegerToStr(T value)
{
    memset(number,0,sizeof(number));
    num_size = 0;
    while(value){
        short b = value % 10;
        value = value / 10;
        number[num_size++] = b+'0';
    }
    for( int i=0; i < num_size/2; ++i ){
        std::swap(number[i], number[num_size-1-i]);
    }
}
