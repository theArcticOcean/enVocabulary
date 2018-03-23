#ifndef ENDATA_H
#define ENDATA_H

#define ENWORD_SEARCH_ENTRY     "https://api.shanbay.com/bdc/search/"
#define SENTENCE_SEARCH_ENTRY   "https://api.shanbay.com/bdc/example/"
#define DOWNFILES_PATH          "./Down"
#define DOWNFILES_PICS_PATH     "./Down/Pics"
#define DOWNFILES_WORDS_PATH    "./Down/Words"
#define TOKEN_STR               "access_token=NC5Bk1jVBoALcVcDuh05hkBVgi0wG5"
#define SENTENCE_TABLE          "Statement"

#define MAX_PICS_NUMBER             200
#define MAX_WORD_NUMBER             20
#define NUM_ERROR                   1e-6
#define BUFFER_LEN                  1024*1024*8
#define SENTENCE_NUM                7
#define COLLECT_SENTENCE_PAGESIZE   6
#define COLLECT_WORD_PAGESIZE       6

/*
CREATE TABLE IF NOT EXISTS Statement
(sentences text PRIMARY KEY, translation text)

CREATE TABLE IF NOT EXISTS Vocabulary
(myWord text, translation text, secs INTEGER, PRIMARY KEY(myWord, secs))
*/

#include <pthread.h>
#include <string>
#include <vector>
#include <QJsonObject>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <map>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <QDebug>
#include <QDebugStateSaver>
using namespace std;

typedef struct _enWordInfo{
    QString uk_audio_addresses;
    QString us_audio_addresses;
    QString cn_definition;
    QString en_definition;
    QString uk_pronunciation;
    QString us_pronunciation;
    double vocabulary_id;

    void init() {
        uk_audio_addresses = "";
        us_audio_addresses = "";
        cn_definition = "";
        en_definition = "";
        uk_pronunciation = "";
        us_pronunciation = "";
        vocabulary_id = 0;
    }
}enWordInfo;

typedef struct  __sentenceUnit{
    QString sentence;
    QString translation;
    __sentenceUnit() {
        sentence = "";
        translation = "";
    }
    __sentenceUnit(QString _sentence, QString _translation) {
        sentence = _sentence;
        translation = _translation;
    }
}sentenceUnit;

typedef struct __wordUnit{
    string word;
    string translation;
    __wordUnit(){
        word = "";
        translation = "";
    }
    __wordUnit(string _word, string _translation){
        word = _word;
        translation = _translation;
    }
    const char *c_str(){
        string str = word;
        str = str+"\n";
        str = str+translation;
        str = str+"\n";
        return str.c_str();
    }
    int size(){
        return word.length()+translation.length();
    }
    bool operator ==(const __wordUnit another){
        return word == another.word && translation == another.translation ;
    }
    friend ifstream& operator >>(ifstream &in,__wordUnit &myWord){
        in>>myWord.word;
        string str;
        in>>myWord.translation;
        while(!in.eof()){
            in>>str;
            myWord.translation += " ";
            myWord.translation += str;
        }
        return in;
    }
    __wordUnit& operator =(const __wordUnit &myWord){
        word = myWord.word;
        translation = myWord.translation;
        return *this;
    }
    friend ofstream& operator <<(ofstream &out,__wordUnit &myWord){
        out<<myWord.word<<"\n";
        out<<myWord.translation;
        return out;
    }
    friend ostream& operator <<(ostream &out, __wordUnit &myWord){
        out<<myWord.word<<"\n";
        out<<myWord.translation;
        return out;
    }
    friend QDebug& operator <<(QDebug debug,const __wordUnit &myWord){
        QDebugStateSaver saver(debug);
        debug.nospace() << myWord.word.c_str() << "\n" << myWord.translation.c_str();
        return debug;
    }
}wordUnit;

typedef enum _srcEnum{
    SearchUI,
    MySentenceUI,
    CollectWordUI,
    CollectSentenceUI,
    MaxSrc
}srcEnum;

class enData
{    
    static enData *instance;
    static pthread_mutex_t instanceMutex;
    QSqlDatabase db;
    QSqlQuery *query;
    char number[25];
    int num_size;

public:
    enData();
    ~enData();
    void picsCheck();
    void jsonParseForWord(const QJsonObject cjson);
    void jsonParseForSentence(const QJsonObject cjson);
    void wordInfShow();
    void sentencesShow();
    void addSentenceToDB(const int index);
    bool addWordToDB(QString str);
    void deleteSentenceFromDB(const int index);
    void deleteSentenceFromDB(const QString text);
    bool checkSentenceInDB(const int index);
    bool checkWordInDB(string str);
    void getCollectSentencePage(const int index);
    wordUnit readWordFile(const char *path);
    bool wordStoreLimit();
    void getCollectWordFromDisc();
    int getColSentencePageCount();
    int getColWordPageCount();
    QString simpleSentence(const QString sentence);
    int getSentenceCount() const;
    bool checkElementInJson(QJsonObject &json, const string key);
    void showTableVocabulary();
    void showTableSentence();

    static enData* getInstance();
    static bool strIsNum(string str);

    template <typename T>
    void uintegerToStr(T value);

    enWordInfo wordInf;
    vector<sentenceUnit> v_sentences;
    vector<sentenceUnit> v_collectSentences;
    map<ulong, wordUnit> m_collectWords;
};

#endif // ENDATA_H
