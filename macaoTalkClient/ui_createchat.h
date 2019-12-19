/********************************************************************************
** Form generated from reading UI file 'createchat.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATECHAT_H
#define UI_CREATECHAT_H

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

class Ui_CreateChat
{
public:
    QLineEdit *name_line;
    QPushButton *connect_btn;
    QPushButton *cancel_btn;
    QLabel *info_label;
    QLabel *label_2;

    void setupUi(QDialog *CreateChat)
    {
        if (CreateChat->objectName().isEmpty())
            CreateChat->setObjectName(QStringLiteral("CreateChat"));
        CreateChat->resize(251, 214);
        name_line = new QLineEdit(CreateChat);
        name_line->setObjectName(QStringLiteral("name_line"));
        name_line->setGeometry(QRect(20, 130, 211, 27));
        connect_btn = new QPushButton(CreateChat);
        connect_btn->setObjectName(QStringLiteral("connect_btn"));
        connect_btn->setGeometry(QRect(130, 170, 99, 27));
        cancel_btn = new QPushButton(CreateChat);
        cancel_btn->setObjectName(QStringLiteral("cancel_btn"));
        cancel_btn->setGeometry(QRect(20, 170, 99, 27));
        info_label = new QLabel(CreateChat);
        info_label->setObjectName(QStringLiteral("info_label"));
        info_label->setGeometry(QRect(6, 40, 241, 41));
        info_label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(CreateChat);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 100, 201, 20));

        retranslateUi(CreateChat);

        QMetaObject::connectSlotsByName(CreateChat);
    } // setupUi

    void retranslateUi(QDialog *CreateChat)
    {
        CreateChat->setWindowTitle(QApplication::translate("CreateChat", "Dialog", 0));
        connect_btn->setText(QApplication::translate("CreateChat", "Connect", 0));
        cancel_btn->setText(QApplication::translate("CreateChat", "Cancel", 0));
        info_label->setText(QApplication::translate("CreateChat", "Wait...", 0));
        label_2->setText(QApplication::translate("CreateChat", "Insert your partner's name", 0));
    } // retranslateUi

};

namespace Ui {
    class CreateChat: public Ui_CreateChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATECHAT_H
