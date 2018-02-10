#ifndef MYSENTENCES_H
#define MYSENTENCES_H

#include <QWidget>
#include <QShowEvent>
#include <QThread>
#include <QLabel>
#include <vector>
#include <QVBoxLayout>
#include <QHBoxLayout>
using namespace std;

namespace Ui {
class mySentences;
}

template<typename T>
void deleteObject(T *obj){
    delete obj;
    obj = NULL;
}

class mySentences : public QWidget
{
    Q_OBJECT

public:
    explicit mySentences(QWidget *parent = 0);
    ~mySentences();

public slots:
    void slotUpdateSentences();
private slots:
    void on_pushButton_clicked();

private:
    Ui::mySentences *ui;
    QHBoxLayout *hLayout;
};

#endif // MYSENTENCES_H
