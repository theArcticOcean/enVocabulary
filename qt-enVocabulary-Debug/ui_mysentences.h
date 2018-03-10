/********************************************************************************
** Form generated from reading UI file 'mysentences.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYSENTENCES_H
#define UI_MYSENTENCES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mySentences
{
public:
    QPushButton *backButton;

    void setupUi(QWidget *mySentences)
    {
        if (mySentences->objectName().isEmpty())
            mySentences->setObjectName(QStringLiteral("mySentences"));
        mySentences->resize(621, 592);
        backButton = new QPushButton(mySentences);
        backButton->setObjectName(QStringLiteral("backButton"));
        backButton->setGeometry(QRect(580, 10, 31, 31));

        retranslateUi(mySentences);

        QMetaObject::connectSlotsByName(mySentences);
    } // setupUi

    void retranslateUi(QWidget *mySentences)
    {
        mySentences->setWindowTitle(QApplication::translate("mySentences", "Form", Q_NULLPTR));
        backButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class mySentences: public Ui_mySentences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYSENTENCES_H
