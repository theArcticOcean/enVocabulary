/********************************************************************************
** Form generated from reading UI file 'collectwords.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLLECTWORDS_H
#define UI_COLLECTWORDS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_collectWords
{
public:
    QPushButton *backButton;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_1;
    QLabel *label1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label6;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *previousButton;
    QLabel *label;
    QPushButton *nextButton;

    void setupUi(QWidget *collectWords)
    {
        if (collectWords->objectName().isEmpty())
            collectWords->setObjectName(QStringLiteral("collectWords"));
        collectWords->resize(603, 557);
        backButton = new QPushButton(collectWords);
        backButton->setObjectName(QStringLiteral("backButton"));
        backButton->setGeometry(QRect(570, 10, 31, 31));
        groupBox = new QGroupBox(collectWords);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(30, 40, 531, 471));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"background-color: rgb(230, 255, 230)\n"
"}\n"
"QLabel:hover{\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"stop: 0 #FAFBFE, stop: 1 #DCDEF1);/*\350\277\231\346\230\257\344\270\200\344\270\252\346\270\220\345\217\230\351\242\234\350\211\262*/\n"
"color: rgb(138,43,226);\n"
"}"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_1 = new QHBoxLayout();
        horizontalLayout_1->setObjectName(QStringLiteral("horizontalLayout_1"));
        label1 = new QLabel(groupBox);
        label1->setObjectName(QStringLiteral("label1"));

        horizontalLayout_1->addWidget(label1);

        horizontalLayout_1->setStretch(0, 25);

        verticalLayout->addLayout(horizontalLayout_1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label2 = new QLabel(groupBox);
        label2->setObjectName(QStringLiteral("label2"));

        horizontalLayout_2->addWidget(label2);

        horizontalLayout_2->setStretch(0, 25);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label3 = new QLabel(groupBox);
        label3->setObjectName(QStringLiteral("label3"));

        horizontalLayout_3->addWidget(label3);

        horizontalLayout_3->setStretch(0, 25);

        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label4 = new QLabel(groupBox);
        label4->setObjectName(QStringLiteral("label4"));

        horizontalLayout_4->addWidget(label4);

        horizontalLayout_4->setStretch(0, 25);

        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label5 = new QLabel(groupBox);
        label5->setObjectName(QStringLiteral("label5"));

        horizontalLayout_5->addWidget(label5);

        horizontalLayout_5->setStretch(0, 25);

        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label6 = new QLabel(groupBox);
        label6->setObjectName(QStringLiteral("label6"));

        horizontalLayout_6->addWidget(label6);

        horizontalLayout_6->setStretch(0, 25);

        verticalLayout->addLayout(horizontalLayout_6);

        layoutWidget_2 = new QWidget(collectWords);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(190, 520, 230, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget_2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        previousButton = new QPushButton(layoutWidget_2);
        previousButton->setObjectName(QStringLiteral("previousButton"));

        horizontalLayout->addWidget(previousButton);

        label = new QLabel(layoutWidget_2);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        nextButton = new QPushButton(layoutWidget_2);
        nextButton->setObjectName(QStringLiteral("nextButton"));

        horizontalLayout->addWidget(nextButton);


        retranslateUi(collectWords);

        QMetaObject::connectSlotsByName(collectWords);
    } // setupUi

    void retranslateUi(QWidget *collectWords)
    {
        collectWords->setWindowTitle(QApplication::translate("collectWords", "Form", Q_NULLPTR));
        backButton->setText(QString());
        groupBox->setTitle(QApplication::translate("collectWords", "words", Q_NULLPTR));
        label1->setText(QApplication::translate("collectWords", "TextLabel", Q_NULLPTR));
        label2->setText(QApplication::translate("collectWords", "TextLabel", Q_NULLPTR));
        label3->setText(QApplication::translate("collectWords", "TextLabel", Q_NULLPTR));
        label4->setText(QApplication::translate("collectWords", "TextLabel", Q_NULLPTR));
        label5->setText(QApplication::translate("collectWords", "TextLabel", Q_NULLPTR));
        label6->setText(QApplication::translate("collectWords", "TextLabel", Q_NULLPTR));
        previousButton->setText(QString());
        label->setText(QApplication::translate("collectWords", "TextLabel", Q_NULLPTR));
        nextButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class collectWords: public Ui_collectWords {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLLECTWORDS_H
