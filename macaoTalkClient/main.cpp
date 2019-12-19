#include "mainwindow.h"
#include <QApplication>

/*copy from templete*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 6058 /* 서버 포트 */
#define SERVER_IP "127.0.0.1"

int main(int argc, char *argv[])
{
    int csock;
    struct sockaddr_in serv_addr;
    /* 클라이언트 소켓 생성 */
    if ((csock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    /* 서버 주소 설정 */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serv_addr.sin_port = htons(PORT);
    /* 서버 연결 */
    if (connect(csock, (struct sockaddr *)&serv_addr,
                sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }

    QApplication a(argc, argv);
    MainWindow w(csock, 0);
    w.setLogin();
    w.setGeometry(500, 500, w.width(), w.height());
    w.show();

    return a.exec();
}
