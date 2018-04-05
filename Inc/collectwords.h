#ifndef COLLECTWORDS_H
#define COLLECTWORDS_H

#include <QWidget>

namespace Ui {
class collectWords;
}

class collectWords : public QWidget
{
    Q_OBJECT

public:
    explicit collectWords(QWidget *parent = 0);
    ~collectWords();
    void fillWordInLabels(const int index, QString str);

    QPoint lastPos;
protected:
    virtual void showEvent(QShowEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
private slots:
    void on_backButton_clicked();

    void on_nextButton_clicked();

    void on_previousButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::collectWords *ui;
    int pageIndex;
};

#endif // COLLECTWORDS_H
