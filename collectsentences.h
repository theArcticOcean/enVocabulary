#ifndef COLLECTSENTENCES_H
#define COLLECTSENTENCES_H

#include <QWidget>

namespace Ui {
class collectSentences;
}

class collectSentences : public QWidget
{
    Q_OBJECT
    int pageIndex;
    int pageCount;
public:
    explicit collectSentences(QWidget *parent = 0);
    ~collectSentences();
    void fillSentenceInCheckBox(const int index, QString str);
protected:
    virtual void showEvent(QShowEvent *event);
private slots:
    void on_backButton_clicked();

private:
    Ui::collectSentences *ui;
};

#endif // COLLECTSENTENCES_H
