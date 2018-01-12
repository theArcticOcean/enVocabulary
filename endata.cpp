#include "endata.h"
#include "log.h"
#include <queue>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <QJsonParseError>
#include <QJsonArray>
using namespace std;

pthread_mutex_t enData::instanceMutex = PTHREAD_MUTEX_INITIALIZER;
enData* enData::instance = NULL;

enData::enData()
{
    pthread_mutex_init(&instanceMutex, NULL);
    if(0 != access(DOWNFILES_PATH, F_OK)){
        if(0 != mkdir(DOWNFILES_PATH, 0777)){
            LOGDBG("mkdir failed: %s", strerror(errno));
        }
    }
    if(0 != access(DOWNFILES_PICS_PATH, F_OK)){
        if(0 != mkdir(DOWNFILES_PICS_PATH, 0777)){
            LOGDBG("mkdir failed: %s", strerror(errno));
        }
    }
    picsCheck();
    wordInf.init();
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
        wordInf.uk_audio_addresses = audioArray[0].toString().toStdString();
        audioArray = jsonTmp["us"].toArray();
        wordInf.us_audio_addresses = audioArray[0].toString().toStdString();

        jsonTmp = json["data"].toObject();
        if(!checkElementInJson(jsonTmp, "cn_definition")) return;
        value = jsonTmp["cn_definition"];
        jsonTmp = value.toObject();
        if(!checkElementInJson(jsonTmp, "defn")) return;
        wordInf.cn_definition = jsonTmp["defn"].toString().toStdString();
        jsonTmp = json["data"].toObject();
        if(!checkElementInJson(jsonTmp, "en_definition")) return;
        value = jsonTmp["en_definition"];
        jsonTmp = value.toObject();
        if(!checkElementInJson(jsonTmp, "defn")) return;
        wordInf.en_definition = jsonTmp["defn"].toString().toStdString();

        jsonTmp = json["data"].toObject();
        if(!checkElementInJson(jsonTmp, "pronunciations")) return;
        jsonTmp = jsonTmp["pronunciations"].toObject();
        if(!checkElementInJson(jsonTmp, "uk")) return;
        wordInf.uk_pronunciation = jsonTmp["uk"].toString().toStdString();
        if(!checkElementInJson(jsonTmp, "us")) return;
        wordInf.us_pronunciation = jsonTmp["us"].toString().toStdString();

        jsonTmp = json["data"].toObject();
        if(!checkElementInJson(jsonTmp, "uk_audio")) return;
        wordInf.uk_audio_addresses = jsonTmp["uk_audio"].toString().toStdString();
        if(!checkElementInJson(jsonTmp, "us_audio")) return;
        wordInf.us_audio_addresses = jsonTmp["us_audio"].toString().toStdString();
        wordInfShow();
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
    ret += sprintf(buff+ret,"%s\n",wordInf.cn_definition.c_str());
    ret += sprintf(buff+ret,"%s","en_definition: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.en_definition.c_str());
    ret += sprintf(buff+ret,"%s","uk_audio_addresses: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.uk_audio_addresses.c_str());
    ret += sprintf(buff+ret,"%s","uk_pronunciation: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.uk_pronunciation.c_str());
    ret += sprintf(buff+ret,"%s","us_audio_addresses: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.us_audio_addresses.c_str());
    ret += sprintf(buff+ret,"%s","us_pronunciation: ");
    ret += sprintf(buff+ret,"%s\n",wordInf.us_pronunciation.c_str());
    LOGDBG("\n%s",buff);
}
