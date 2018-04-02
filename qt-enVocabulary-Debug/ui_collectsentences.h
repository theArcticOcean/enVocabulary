/********************************************************************************
** Form generated from reading UI file 'collectsentences.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLLECTSENTENCES_H
#define UI_COLLECTSENTENCES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_collectSentences
{
public:
    QPushButton *deleteButton;
    QCheckBox *checkBox_7;
    QPushButton *backButton;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QPushButton *previousButton;
    QLabel *label;
    QPushButton *nextButton;

    void setupUi(QWidget *collectSentences)
    {
        if (collectSentences->objectName().isEmpty())
            collectSentences->setObjectName(QStringLiteral("collectSentences"));
        collectSentences->resize(618, 569);
        deleteButton = new QPushButton(collectSentences);
        deleteButton->setObjectName(QStringLiteral("deleteButton"));
        deleteButton->setGeometry(QRect(520, 10, 61, 32));
        checkBox_7 = new QCheckBox(collectSentences);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));
        checkBox_7->setGeometry(QRect(40, 20, 87, 20));
        backButton = new QPushButton(collectSentences);
        backButton->setObjectName(QStringLiteral("backButton"));
        backButton->setGeometry(QRect(580, 10, 31, 31));
        groupBox = new QGroupBox(collectSentences);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(40, 40, 541, 471));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"background-color: rgb(230, 255, 230)\n"
"}\n"
"QCheckBox:hover{\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"stop: 0 #FAFBFE, stop: 1 #DCDEF1);/*\350\277\231\346\230\257\344\270\200\344\270\252\346\270\220\345\217\230\351\242\234\350\211\262*/\n"
"color: rgb(138,43,226);\n"
"/*background-color: rgb(220, 255, 222) */\n"
"}\n"
"QCheckBox:checked {\n"
"border: none;\n"
"border-style: none;\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"stop: 0 #FAFBFE, stop: 1 #DCDEF1);/*\350\277\231\346\230\257\344\270\200\344\270\252\346\270\220\345\217\230\351\242\234\350\211\262*/\n"
"color: rgb(138,43,226);\n"
"}"));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 20, 541, 451));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        checkBox = new QCheckBox(layoutWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(13);
        checkBox->setFont(font);
        checkBox->setStyleSheet(QStringLiteral(""));

        verticalLayout->addWidget(checkBox);

        checkBox_2 = new QCheckBox(layoutWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        sizePolicy.setHeightForWidth(checkBox_2->sizePolicy().hasHeightForWidth());
        checkBox_2->setSizePolicy(sizePolicy);
        checkBox_2->setStyleSheet(QStringLiteral(""));

        verticalLayout->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(layoutWidget);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        sizePolicy.setHeightForWidth(checkBox_3->sizePolicy().hasHeightForWidth());
        checkBox_3->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(layoutWidget);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        sizePolicy.setHeightForWidth(checkBox_4->sizePolicy().hasHeightForWidth());
        checkBox_4->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(checkBox_4);

        checkBox_5 = new QCheckBox(layoutWidget);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        sizePolicy.setHeightForWidth(checkBox_5->sizePolicy().hasHeightForWidth());
        checkBox_5->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(checkBox_5);

        checkBox_6 = new QCheckBox(layoutWidget);
        checkBox_6->setObjectName(QStringLiteral("checkBox_6"));
        sizePolicy.setHeightForWidth(checkBox_6->sizePolicy().hasHeightForWidth());
        checkBox_6->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(checkBox_6);

        layoutWidget1 = new QWidget(collectSentences);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(200, 520, 230, 32));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        previousButton = new QPushButton(layoutWidget1);
        previousButton->setObjectName(QStringLiteral("previousButton"));

        horizontalLayout->addWidget(previousButton);

        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        nextButton = new QPushButton(layoutWidget1);
        nextButton->setObjectName(QStringLiteral("nextButton"));

        horizontalLayout->addWidget(nextButton);


        retranslateUi(collectSentences);

        QMetaObject::connectSlotsByName(collectSentences);
    } // setupUi

    void retranslateUi(QWidget *collectSentences)
    {
        collectSentences->setWindowTitle(QApplication::translate("collectSentences", "Form", Q_NULLPTR));
        deleteButton->setText(QApplication::translate("collectSentences", "\345\210\240\351\231\244", Q_NULLPTR));
        checkBox_7->setText(QApplication::translate("collectSentences", "\345\205\250\351\200\211", Q_NULLPTR));
        backButton->setText(QString());
        groupBox->setTitle(QApplication::translate("collectSentences", "sentences", Q_NULLPTR));
        checkBox->setText(QApplication::translate("collectSentences", "CheckBox", Q_NULLPTR));
        checkBox_2->setText(QApplication::translate("collectSentences", "CheckBox", Q_NULLPTR));
        checkBox_3->setText(QApplication::translate("collectSentences", "CheckBox", Q_NULLPTR));
        checkBox_4->setText(QApplication::translate("collectSentences", "CheckBox", Q_NULLPTR));
        checkBox_5->setText(QApplication::translate("collectSentences", "CheckBox", Q_NULLPTR));
        checkBox_6->setText(QApplication::translate("collectSentences", "CheckBox", Q_NULLPTR));
        previousButton->setText(QString());
        label->setText(QApplication::translate("collectSentences", "TextLabel", Q_NULLPTR));
        nextButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class collectSentences: public Ui_collectSentences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLLECTSENTENCES_H
