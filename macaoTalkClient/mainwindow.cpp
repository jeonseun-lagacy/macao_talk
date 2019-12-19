#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "login.h"
#include "createchat.h"
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <fstream>
#include <QSocketNotifier>
#include <QLabel>
#include <QBoxLayout>
#include <QFileDialog>

#define MAXDATASIZE 128

MainWindow::MainWindow(int csock, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->csock = csock;
    socnot = new QSocketNotifier(csock, QSocketNotifier::Read, this);
    connect(socnot, SIGNAL( activated(int)), this, SLOT( wait_chat()));
    ui->scrollArea->setWidgetResizable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setLogin(){
    Login login_dialog;
    login_dialog.csock = this->csock;
    if(login_dialog.exec()){
        memset(usr, 0, 128);
        strcpy(this->usr, login_dialog.usr);
        ui->label_2->setText(this->usr);
        setWaitMode();
        //run next step
    }
    else{   //exit application
        exit(0);
    }
}

void MainWindow::wait_chat(){
    char buf[MAXDATASIZE];
    char *data_buf;
    int numbytes;
    int size = 0;

    memset(buf, 0, MAXDATASIZE);
    if ((numbytes = recv(csock, buf, MAXDATASIZE, 0)) == -1)
    {
        perror("recv");
        exit(1);
    }
    buf[numbytes] = '\0';

    data_buf = strtok(buf, "=");
    if (!strcmp(data_buf, "failed_type"))
    {
        printf("send type err");
    }
    else if (!strcmp(data_buf, "disconn"))  //disconnect chat
    {
        printf("disconnect");
        setWaitMode();
    }
    else if (!strcmp(data_buf, "success"))  //create chat
    {
        while (data_buf != NULL)
        {
            data_buf = strtok(NULL, "=");
            if (data_buf != NULL)
            {
                strcpy(this->partner, data_buf);
                setChatMode();
            }
        }
    }
    else if (!strcmp(data_buf, "msg"))  //get chat msg
    {
        while (data_buf != NULL)
        {
            data_buf = strtok(NULL, "=");
            if (data_buf != NULL)
            {
                char msg_buf[512];
                memset(msg_buf, 0, 512);
                sprintf(msg_buf, "%s> %s", this->partner, data_buf);
                QString value = msg_buf;
                QLabel *label = new QLabel;
                label->setText(value);
                label->setAlignment(Qt::AlignLeft);
                ui->scroll_layout->addWidget(label);
            }
        }
    }
    else if (!strcmp(data_buf, "file")) //get file
    {
        int c = 0;
        int filesize = 5000;
        char f[filesize];
        char dir[2048];
        int filehandle;

        while (data_buf != NULL)
        {
            data_buf = strtok(NULL, "=");
            if (data_buf != NULL)
            {
                memset(dir, 0, 2048);
                sprintf(dir, "./download/%s", data_buf);
                filehandle = open(dir, O_WRONLY | O_CREAT | O_TRUNC/**/, 0777);
                while (1) {
                    memset(f, 0, filesize);
                    if ((numbytes = recv(csock, f, filesize, 0)) == -1)
                    {
                        perror("recv");
                        exit(1);
                    }
                    if(!strcmp(f, "finish")){
                        break;
                    }
                    write(filehandle, f, numbytes );
                    memset(f, 0, filesize);
                    sprintf(f, "check");
                    send(csock, f, strlen(f), 0);
                }
                ::close(filehandle);
                char msg_buf[512];
                memset(msg_buf, 0, 512);
                sprintf(msg_buf, "%s> %s\nDownload complete!", this->partner, data_buf);
                QString value = msg_buf;
                QLabel *label = new QLabel;
                label->setText(value);
                label->setAlignment(Qt::AlignLeft);
                ui->scroll_layout->addWidget(label);
            }
        }
    }
}

void MainWindow::setWaitMode(){
    ui->info_label->show();
    ui->create_btn->show();

    ui->file_btn->hide();
    ui->send_btn->hide();
    ui->scrollArea->hide();
    ui->send_edit->hide();
}

void MainWindow::setChatMode(){
    ui->info_label->hide();
    ui->create_btn->hide();

    ui->file_btn->show();
    ui->send_btn->show();
    ui->scrollArea->show();
    ui->send_edit->show();
}

void MainWindow::on_create_btn_clicked()
{
    socnot->setEnabled(false);
    CreateChat create_chat;
    create_chat.csock = this->csock;
    if(create_chat.exec()){
        //create chat successfully
        memset(this->partner, 0, 128);
        strcpy(this->partner, create_chat.partner);
        setChatMode();
        socnot->setEnabled(true);
    }
    else{
        //cancel create chat
    }
}

void MainWindow::on_send_btn_clicked()
{
    char buf[MAXDATASIZE];
    char input[128];
    int len;

    QString str = ui->send_edit->text();
    strcpy(input, str.toUtf8());
    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "msg=%s", input);
    len = strlen(buf);
    if (send(csock, buf, len, 0) != len)
    {
        fprintf(stderr, "send failed...\n");
        exit(1);
    }
    ui->send_edit->setText("");
    char msg_buf[512];
    memset(msg_buf, 0, 512);
    sprintf(msg_buf, "%s<", input);
    QString value = msg_buf;
    value = value.rightJustified(80, ' ');
    QLabel *label = new QLabel;
    label->setText(value);
    label->setAlignment(Qt::AlignRight);
    ui->scroll_layout->addWidget(label);
}

void MainWindow::on_file_btn_clicked()
{
    QStringList list;
    char name_buf[512];
    char buf[MAXDATASIZE];
    int filesize = 5000;
    struct stat obj;
    int filehandle;
    int len;
    int size = 0;

    QFileDialog dlg(this,tr("Open File"), "/home");
    if(dlg.exec()){
        list = dlg.selectedFiles();
    }
    strcpy(name_buf, list[0].toUtf8());
    char *file = basename(name_buf);

    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "file=%s", file);
    len = strlen(buf);
    if (send(csock, buf, len, 0) != len)
    {
        fprintf(stderr, "send failed...\n");
        exit(1);
    }

    //std::ifstream in_file(name_buf);
    filehandle = open(name_buf, O_RDONLY, 0);

    //while (!in_file.eof()) {
    while (1) {
        char file_buf[filesize];
        memset(file_buf, 0, filesize);
        size = 0;
        size = read(filehandle, file_buf, filesize);
        if (size > 0){
            if (send(csock, file_buf, size, 0) != size)
            {
                fprintf(stderr, "file send failed...\n");
                //exit(1);
            }
            recv(csock, buf, MAXDATASIZE, 0);
        }
        else{
            break;
        }
    }
    ::close(filehandle);

    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "finish");
    len = strlen(buf);
    if (send(csock, buf, len, 0) != len)
    {
        fprintf(stderr, "send failed...\n");
        exit(1);
    }

    char msg_buf[512];
    memset(msg_buf, 0, 512);
    sprintf(msg_buf, "%s send to %s<", file, this->partner);
    QString value = msg_buf;
    QLabel *label = new QLabel;
    label->setText(value);
    label->setAlignment(Qt::AlignRight);
    ui->scroll_layout->addWidget(label);
}

void MainWindow::on_exit_btn_clicked()
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
    exit(0);
}
