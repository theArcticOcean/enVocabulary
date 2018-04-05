#ifndef MYSENTENCES_H
#define MYSENTENCES_H

#include <QWidget>
#include <QShowEvent>
#include <QThread>
#include <QLabel>
#include <vector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

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
    void builtHButtonSigAndSLot();

    QPoint lastPos;
protected:
    virtual void showEvent(QShowEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    void updateSentences();
private slots:
    void on_backButton_clicked();
public slots:
    void on_collectClick();
private:
    Ui::mySentences *ui;
    vector<QHBoxLayout*> sentenceHLays;
    vector<QLabel*> v_sentenceLabel;
    vector<QPushButton*> heartButtons;
    QVBoxLayout *vLayout;
    QLabel *backLabel;
};

#endif // MYSENTENCES_H
