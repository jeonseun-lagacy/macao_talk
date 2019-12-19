#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    int csock;
    char usr[128];

private slots:
    void on_exit_btn_clicked();

    void on_login_btn_clicked();

    void on_join_btn_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
