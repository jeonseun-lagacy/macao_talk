#include "createchat.h"
#include "ui_createchat.h"

#include <stdio.h>
#include <sys/socket.h>

#define MAXDATASIZE 128

CreateChat::CreateChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateChat)
{
    ui->setupUi(this);
}

CreateChat::~CreateChat()
{
    delete ui;
}
void CreateChat::on_cancel_btn_clicked()
{
    reject();
}

void CreateChat::on_connect_btn_clicked()
{
    char buf[MAXDATASIZE];
    char input[128];
    int len;
    int numbytes;

    QString str = ui->name_line->text();
    strcpy(input, str.toUtf8());
    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "conn=%s", input);
    len = strlen(buf);
    memset(partner, 0, 128);
    strcpy(partner, input);
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
        ui->info_label->setText("Matching failed\nThis user do not exist or as Offline!");
    }
}
