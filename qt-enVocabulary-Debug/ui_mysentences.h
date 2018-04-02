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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mySentences
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *backButton;

    void setupUi(QWidget *mySentences)
    {
        if (mySentences->objectName().isEmpty())
            mySentences->setObjectName(QStringLiteral("mySentences"));
        mySentences->resize(621, 592);
        widget = new QWidget(mySentences);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(20, 10, 591, 32));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(378, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        backButton = new QPushButton(widget);
        backButton->setObjectName(QStringLiteral("backButton"));

        horizontalLayout->addWidget(backButton);

        horizontalLayout->setStretch(0, 20);
        horizontalLayout->setStretch(1, 1);

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
