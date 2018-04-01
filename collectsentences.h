#ifndef COLLECTSENTENCES_H
#define COLLECTSENTENCES_H

#include <QWidget>
#include <QCheckBox>

namespace Ui {
class collectSentences;
}

class collectSentences : public QWidget
{
    Q_OBJECT
    int pageIndex;
public:
    explicit collectSentences(QWidget *parent = 0);
    ~collectSentences();
    void fillSentenceInCheckBox(const int index, QString str);
    void connectCheckBoxClickAndShowDelBtn();

    QPoint lastPos;
protected:
    virtual void showEvent(QShowEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    void setCheckBoxChecked(QCheckBox *box, bool checked);
public slots:
    void showDeleteButton();
private slots:
    void on_backButton_clicked();
    void on_deleteButton_clicked();
    void on_nextButton_clicked();

    void on_previousButton_clicked();

    void on_checkBox_7_clicked();

private:
    Ui::collectSentences *ui;
};

#endif // COLLECTSENTENCES_H
