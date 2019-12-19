#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <QMainWindow>
#include <QSocketNotifier>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int csock, QWidget *parent = 0);
    ~MainWindow();
    QSocketNotifier *socnot;
    void setLogin();
    void setWaitMode();
    void setChatMode();
    int csock;
    char usr[128];
    char partner[128];

private slots:
    void on_create_btn_clicked();

    void on_send_btn_clicked();

    void on_file_btn_clicked();

    void on_exit_btn_clicked();

    void wait_chat();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
