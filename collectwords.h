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
    void connectCheckBoxClickAndShowDelBtn();
    void fillWordInCheckBox(const int index, QString str);

    QPoint lastPos;
protected:
    virtual void showEvent(QShowEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
public slots:
    void showDeleteButton();
private slots:
    void on_deleteButton_clicked();

    void on_backButton_clicked();

private:
    Ui::collectWords *ui;
    int pageIndex;
    int pageCount;
};

#endif // COLLECTWORDS_H
