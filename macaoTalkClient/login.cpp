#include "login.h"
#include "ui_login.h"
#include <stdio.h>
#include <sys/socket.h>

#define MAXDATASIZE 128

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_exit_btn_clicked()
{
    char buf[MAXDATASIZE];
    int len;

    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "exit");
    len = strlen(buf);
    if (send(csock, buf, len, 0) != len)
    {
        fprintf(stderr, "send failed...\n");
        exit(1);
    }
    reject();
}

void Login::on_login_btn_clicked()
{
    char buf[MAXDATASIZE];
    char *data_buf;
    char input[128];
    int len;
    int numbytes;
    int size = 0;

    QString str = ui->name_line->text();
    strcpy(input, str.toUtf8());
    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "enter_usr=%s", input);
    len = strlen(buf);
    memset(usr, 0, 128);
    strcpy(usr, input);
    if (send(csock, buf, len, 0) != len)
    {
        fprintf(stderr, "send failed...\n");
        exit(1);
    }
    ui->name_line->setText("");
    memset(buf, 0, MAXDATASIZE);
    if ((numbytes = recv(csock, buf, MAXDATASIZE, 0)) == -1)
    {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';

    if (!strcmp(buf, "success"))
    {
        accept();
    }
    else if (!strcmp(buf, "failed"))
    {
        ui->info_label->setText("Please join first!!");
    }
}

void Login::on_join_btn_clicked()
{
    char buf[MAXDATASIZE];
    char *data_buf;
    char input[128];
    int len;
    int numbytes;
    int size = 0;

    QString str = ui->name_line->text();
    strcpy(input, str.toUtf8());
    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "init_usr=%s", input);
    len = strlen(buf);
    if (send(csock, buf, len, 0) != len)
    {
        fprintf(stderr, "send failed...\n");
        exit(1);
    }
    ui->name_line->setText("");
    memset(buf, 0, MAXDATASIZE);
    if ((numbytes = recv(csock, buf, MAXDATASIZE, 0)) == -1)
    {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';

    if (!strcmp(buf, "success"))
    {
        ui->info_label->setText("Sign up success!!");
    }
    else if (!strcmp(buf, "failed"))
    {
        ui->info_label->setText("This name is already used");
    }
}
