/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Login
{
public:
    QPushButton *join_btn;
    QPushButton *login_btn;
    QPushButton *exit_btn;
    QLineEdit *name_line;
    QLabel *label;
    QLabel *info_label;

    void setupUi(QDialog *Login)
    {
        if (Login->objectName().isEmpty())
            Login->setObjectName(QStringLiteral("Login"));
        Login->resize(359, 197);
        join_btn = new QPushButton(Login);
        join_btn->setObjectName(QStringLiteral("join_btn"));
        join_btn->setGeometry(QRect(268, 150, 85, 27));
        login_btn = new QPushButton(Login);
        login_btn->setObjectName(QStringLiteral("login_btn"));
        login_btn->setGeometry(QRect(122, 150, 121, 27));
        exit_btn = new QPushButton(Login);
        exit_btn->setObjectName(QStringLiteral("exit_btn"));
        exit_btn->setGeometry(QRect(9, 150, 85, 27));
        name_line = new QLineEdit(Login);
        name_line->setObjectName(QStringLiteral("name_line"));
        name_line->setGeometry(QRect(55, 110, 261, 27));
        label = new QLabel(Login);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(110, 20, 144, 30));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setTextFormat(Qt::AutoText);
        label->setAlignment(Qt::AlignCenter);
        info_label = new QLabel(Login);
        info_label->setObjectName(QStringLiteral("info_label"));
        info_label->setGeometry(QRect(6, 80, 351, 20));
        QFont font1;
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setStrikeOut(false);
        info_label->setFont(font1);
        info_label->setAlignment(Qt::AlignCenter);
        QWidget::setTabOrder(name_line, login_btn);
        QWidget::setTabOrder(login_btn, exit_btn);
        QWidget::setTabOrder(exit_btn, join_btn);

        retranslateUi(Login);

        QMetaObject::connectSlotsByName(Login);
    } // setupUi

    void retranslateUi(QDialog *Login)
    {
        Login->setWindowTitle(QApplication::translate("Login", "Dialog", 0));
        join_btn->setText(QApplication::translate("Login", "Join", 0));
        login_btn->setText(QApplication::translate("Login", "Login", 0));
        exit_btn->setText(QApplication::translate("Login", "Exit", 0));
        name_line->setInputMask(QString());
        name_line->setText(QString());
        name_line->setPlaceholderText(QString());
        label->setText(QApplication::translate("Login", "Macao Talk", 0));
        info_label->setText(QApplication::translate("Login", "Please insert your name!", 0));
    } // retranslateUi

};

namespace Ui {
    class Login: public Ui_Login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
