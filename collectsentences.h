#ifndef COLLECTSENTENCES_H
#define COLLECTSENTENCES_H

#include <QWidget>

namespace Ui {
class collectSentences;
}

class collectSentences : public QWidget
{
    Q_OBJECT

public:
    explicit collectSentences(QWidget *parent = 0);
    ~collectSentences();

private:
    Ui::collectSentences *ui;
};

#endif // COLLECTSENTENCES_H
