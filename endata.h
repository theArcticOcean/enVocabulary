#ifndef ENDATA_H
#define ENDATA_H

#define ENWORD_SEARCH_ENTRY     "https://api.shanbay.com/bdc/search/"
#define SENTENCE_SEARCH_ENTRY   "https://api.shanbay.com/bdc/example/"
#define DOWNFILES_PATH          "./Down"
#define DOWNFILES_PICS_PATH     "./Down/Pics"
#define TOKEN_STR               "access_token=yb7D1mhTLcFsYLI7jfVQUHeTUuoPni"
#define SENTENCE_TABLE          "Statement"

#define MAX_PICS_NUMBER          200
#define NUM_ERROR                1e-6
#define BUFFER_LEN               1024*1024*8
#define SENTENCE_NUM             7
#define COLLECT_SENTENCE_PAGESIZE   6

#include <pthread.h>
#include <string>
#include <vector>
#include <QJsonObject>
#include <QtSql/QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

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

class enData
{    
    static enData *instance;
    static pthread_mutex_t instanceMutex;
    QSqlDatabase db;
    QSqlQuery *query;

public:
    enData();
    ~enData();
    void picsCheck();
    void jsonParseForWord(const QJsonObject cjson);
    void jsonParseForSentence(const QJsonObject cjson);
    void wordInfShow();
    void sentencesShow();
    void addSentenceToDB(const int index);
    void deleteSentenceFromDB(const int index);
    void deleteSentenceFromDB(const QString text);
    bool checkSentenceInDB(const int index);
    void getCollectSentencePage(const int index);
    int getColSentencePageCount();
    QString simpleSentence(const QString sentence);
    int getSentenceCount() const;
    bool checkElementInJson(QJsonObject &json, const string key);
    static enData* getInstance();

    enWordInfo wordInf;
    vector<sentenceUnit> v_sentences;
    vector<sentenceUnit> v_collectSentences;
};

#endif // ENDATA_H
