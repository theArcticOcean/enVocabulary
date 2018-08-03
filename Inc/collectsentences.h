/**********************************************************
*
* @brief    The class is for page of collected sentences
*
* @author   theArcticOcean
***********************************************************/

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
public:
    explicit collectSentences(QWidget *parent = 0);
    ~collectSentences();
    void fillSentenceInCheckBox(const int index, const QString &_str);
    void connectCheckBoxClickAndShowDelBtn();

    QPoint lastPos;

public slots:
    void showDeleteButton();

protected:
    virtual void showEvent(QShowEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    void setCheckBoxChecked(QCheckBox *box, bool checked);

private slots:
    void on_backButton_clicked();
    void on_deleteButton_clicked();
    void on_nextButton_clicked();

    void on_previousButton_clicked();

    void on_checkBox_7_clicked();

    void on_clearButton_clicked();

private:
    Ui::collectSentences *ui;
    int pageIndex;
    int sentenceCountPerPage;
};

#endif // COLLECTSENTENCES_H
