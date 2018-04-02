/********************************************************************************
** Form generated from reading UI file 'search.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCH_H
#define UI_SEARCH_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include "QtQuickWidgets/QQuickWidget"

QT_BEGIN_NAMESPACE

class Ui_Search
{
public:
    QLineEdit *lineEdit;
    QLabel *label;
    QQuickWidget *quickWidget;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *uk_label;
    QLabel *us_label;
    QPushButton *uk_button;
    QPushButton *us_button;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *Search)
    {
        if (Search->objectName().isEmpty())
            Search->setObjectName(QStringLiteral("Search"));
        Search->resize(580, 483);
        Search->setStyleSheet(QLatin1String("QWidget{\n"
"	background-color: rgb(230, 255, 230)\n"
"}"));
        lineEdit = new QLineEdit(Search);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(290, 40, 261, 21));
        label = new QLabel(Search);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(240, 40, 31, 16));
        quickWidget = new QQuickWidget(Search);
        quickWidget->setObjectName(QStringLiteral("quickWidget"));
        quickWidget->setGeometry(QRect(30, 40, 150, 400));
        quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        label_2 = new QLabel(Search);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(240, 90, 60, 16));
        label_3 = new QLabel(Search);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(240, 130, 60, 16));
        uk_label = new QLabel(Search);
        uk_label->setObjectName(QStringLiteral("uk_label"));
        uk_label->setGeometry(QRect(320, 90, 131, 16));
        us_label = new QLabel(Search);
        us_label->setObjectName(QStringLiteral("us_label"));
        us_label->setGeometry(QRect(320, 130, 131, 16));
        uk_button = new QPushButton(Search);
        uk_button->setObjectName(QStringLiteral("uk_button"));
        uk_button->setGeometry(QRect(520, 80, 31, 31));
        uk_button->setStyleSheet(QStringLiteral(""));
        us_button = new QPushButton(Search);
        us_button->setObjectName(QStringLiteral("us_button"));
        us_button->setGeometry(QRect(520, 120, 31, 31));
        textBrowser = new QTextBrowser(Search);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(240, 170, 321, 271));
        textBrowser->setStyleSheet(QLatin1String("QTextBrowser{\n"
"	background-color: rgb(230, 255, 230)\n"
"}"));

        retranslateUi(Search);

        QMetaObject::connectSlotsByName(Search);
    } // setupUi

    void retranslateUi(QWidget *Search)
    {
        Search->setWindowTitle(QApplication::translate("Search", "Search", Q_NULLPTR));
        label->setText(QApplication::translate("Search", "\350\213\261\346\261\211", Q_NULLPTR));
        label_2->setText(QApplication::translate("Search", "\350\213\261\345\274\217\345\217\221\351\237\263\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("Search", "\347\276\216\345\274\217\345\217\221\351\237\263\357\274\232", Q_NULLPTR));
        uk_label->setText(QApplication::translate("Search", "uk", Q_NULLPTR));
        us_label->setText(QApplication::translate("Search", "us", Q_NULLPTR));
        uk_button->setText(QApplication::translate("Search", "uk", Q_NULLPTR));
        us_button->setText(QApplication::translate("Search", "us", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Search: public Ui_Search {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCH_H
