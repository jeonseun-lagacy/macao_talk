/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QLabel *label_2;
    QPushButton *send_btn;
    QLineEdit *send_edit;
    QPushButton *exit_btn;
    QPushButton *create_btn;
    QLabel *info_label;
    QToolButton *file_btn;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *scroll_widget;
    QVBoxLayout *scroll_layout;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 565);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(120, 0, 161, 41));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(306, 20, 81, 20));
        label_2->setAlignment(Qt::AlignCenter);
        send_btn = new QPushButton(centralWidget);
        send_btn->setObjectName(QStringLiteral("send_btn"));
        send_btn->setGeometry(QRect(308, 460, 61, 27));
        send_edit = new QLineEdit(centralWidget);
        send_edit->setObjectName(QStringLiteral("send_edit"));
        send_edit->setGeometry(QRect(30, 460, 241, 27));
        exit_btn = new QPushButton(centralWidget);
        exit_btn->setObjectName(QStringLiteral("exit_btn"));
        exit_btn->setGeometry(QRect(30, 10, 71, 27));
        create_btn = new QPushButton(centralWidget);
        create_btn->setObjectName(QStringLiteral("create_btn"));
        create_btn->setGeometry(QRect(150, 310, 99, 27));
        info_label = new QLabel(centralWidget);
        info_label->setObjectName(QStringLiteral("info_label"));
        info_label->setGeometry(QRect(30, 150, 341, 81));
        info_label->setAlignment(Qt::AlignCenter);
        file_btn = new QToolButton(centralWidget);
        file_btn->setObjectName(QStringLiteral("file_btn"));
        file_btn->setGeometry(QRect(270, 460, 24, 25));
        file_btn->setPopupMode(QToolButton::InstantPopup);
        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(30, 50, 341, 401));
        scrollArea->setMaximumSize(QSize(16777215, 16777215));
        scrollArea->setAutoFillBackground(true);
        scrollArea->setFrameShape(QFrame::StyledPanel);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 339, 399));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy);
        scroll_widget = new QWidget(scrollAreaWidgetContents);
        scroll_widget->setObjectName(QStringLiteral("scroll_widget"));
        scroll_widget->setGeometry(QRect(0, 0, 341, 401));
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scroll_widget->sizePolicy().hasHeightForWidth());
        scroll_widget->setSizePolicy(sizePolicy1);
        scroll_widget->setMaximumSize(QSize(16777215, 16777215));
        scroll_widget->setLayoutDirection(Qt::LeftToRight);
        scroll_widget->setAutoFillBackground(true);
        scroll_layout = new QVBoxLayout(scroll_widget);
        scroll_layout->setSpacing(6);
        scroll_layout->setContentsMargins(11, 11, 11, 11);
        scroll_layout->setObjectName(QStringLiteral("scroll_layout"));
        scroll_layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        scroll_layout->setContentsMargins(-1, -1, -1, 0);
        scrollArea->setWidget(scrollAreaWidgetContents);
        MainWindow->setCentralWidget(centralWidget);
        scrollArea->raise();
        label->raise();
        label_2->raise();
        send_btn->raise();
        send_edit->raise();
        exit_btn->raise();
        create_btn->raise();
        info_label->raise();
        file_btn->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label->setText(QApplication::translate("MainWindow", "Macao Talk", 0));
        label_2->setText(QApplication::translate("MainWindow", "user", 0));
        send_btn->setText(QApplication::translate("MainWindow", "Send", 0));
        exit_btn->setText(QApplication::translate("MainWindow", "Exit", 0));
        create_btn->setText(QApplication::translate("MainWindow", "Create chat", 0));
        info_label->setText(QApplication::translate("MainWindow", "Wait for chat.../", 0));
#ifndef QT_NO_TOOLTIP
        file_btn->setToolTip(QApplication::translate("MainWindow", "send file", 0));
#endif // QT_NO_TOOLTIP
        file_btn->setText(QApplication::translate("MainWindow", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
