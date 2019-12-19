#ifndef CREATECHAT_H
#define CREATECHAT_H

#include <QDialog>

namespace Ui {
class CreateChat;
}

class CreateChat : public QDialog
{
    Q_OBJECT

public:
    explicit CreateChat(QWidget *parent = 0);
    ~CreateChat();
    int csock;
    char partner[128];

private slots:

    void on_cancel_btn_clicked();

    void on_connect_btn_clicked();

private:
    Ui::CreateChat *ui;
};

#endif // CREATECHAT_H
