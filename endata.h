#ifndef ENDATA_H
#define ENDATA_H

#define ENWORD_SEARCH_ENTRY     "https://api.shanbay.com/bdc/search/"
#define ACCESS_TOKEN            "mUIESkRq3QpCXolnS9eOyr5NlGJMul"
#define DOWNFILES_PATH          "./Down"
#define DOWNFILES_PICS_PATH     "./Down/Pics"
#define MAX_PICS_NUMBER          200

#include <pthread.h>
#include <string>
#include <QJsonObject>
using namespace std;

typedef struct _enWordInfo{
    string uk_audio_addresses;
    string us_audio_addresses;
    string cn_definition;
    string en_definition;
    string uk_pronunciation;
    string us_pronunciation;

    void init(){
        uk_audio_addresses = "";
        us_audio_addresses = "";
        cn_definition = "";
        en_definition = "";
        uk_pronunciation = "";
        us_pronunciation = "";
    }
}enWordInfo;

class enData
{    
    static enData *instance;
    static pthread_mutex_t instanceMutex;
    enWordInfo wordInf;
public:
    enData();
    void picsCheck();
    static enData* getInstance();
    void jsonParseForWord(const QJsonObject cjson);
    void wordInfShow();
    bool checkElementInJson(QJsonObject &json, const string key);
};

#endif // ENDATA_H
