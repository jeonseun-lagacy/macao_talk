/* macaoserver.c */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mysql.h>

#define PORT 6058 /* port number */
#define QLEN 6 /* size of request queue*/

int init_usr(MYSQL mysql, char *arg, int fd){   //init user (success = 1, failed = 0)
    char q_buf[512];
    MYSQL_RES *res;
    MYSQL_ROW row;
    printf("run init_user");

    memset(q_buf, 0, 512);
    sprintf(q_buf, "select id from accessor");
    if(mysql_query(&mysql, q_buf)){
        printf("Error %d: %s \n", mysql_errno(&mysql), mysql_error(&mysql));
        exit(1);
    }
    res = mysql_use_result(&mysql);
    while ((row = mysql_fetch_row(res))){
        if(!strcmp(row[0], arg)){
            mysql_free_result(res);
            return 0;
        }
    }
    memset(q_buf, 0, 512);
    sprintf(q_buf, "insert into accessor(id, pw, fd) values('%s', '1234', %d)", arg, fd);
    if(mysql_query(&mysql, q_buf)){
        printf("Error %d: %s \n", mysql_errno(&mysql), mysql_error(&mysql));
        exit(1);
    }
    mysql_free_result(res);
    return 1;
}

int enter_usr(MYSQL mysql, char *arg, int fd){ //login user (success = 1, failed = 0)
    char q_buf[512];
    MYSQL_RES *res;
    MYSQL_ROW row;

    memset(q_buf, 0, 512);
    sprintf(q_buf, "select id from accessor where id = '%s'", arg);
    if(mysql_query(&mysql, q_buf)){
        printf("Error %d: %s \n", mysql_errno(&mysql), mysql_error(&mysql));
        exit(1);
    }
    res = mysql_use_result(&mysql);
    while ((row = mysql_fetch_row(res))){
        if(!strcmp(row[0], arg)){
            mysql_free_result(res);
            memset(q_buf, 0, 512);
            sprintf(q_buf, "update accessor set fd= %d where id = '%s'", fd, arg);
            if(mysql_query(&mysql, q_buf)){
                printf("Error %d: %s \n", mysql_errno(&mysql), mysql_error(&mysql));
                exit(1);
            }
            return 1;
        }
    }
    mysql_free_result(res);
    return 0;
}

void exit_usr(MYSQL mysql, int fd){ //logout user
    char q_buf[512];
    MYSQL_RES *res;
    MYSQL_ROW row;

    memset(q_buf, 0, 512);
    sprintf(q_buf, "update accessor set fd= 0 where fd = %d", fd);
    if(mysql_query(&mysql, q_buf)){
        printf("Error %d: %s \n", mysql_errno(&mysql), mysql_error(&mysql));
        exit(1);
    }
}

int create_chat(MYSQL mysql, char * arg){  //create chat (return target fd)
    char q_buf[512];
    MYSQL_RES *res;
    MYSQL_ROW row;

    memset(q_buf, 0, 512);
    sprintf(q_buf, "select fd from accessor where id = '%s'", arg);
    if(mysql_query(&mysql, q_buf)){
        printf("Error %d: %s \n", mysql_errno(&mysql), mysql_error(&mysql));
        exit(1);
    }
    res = mysql_use_result(&mysql);
    row = mysql_fetch_row(res);
    if(row != NULL){
        int result = atoi(row[0]);
        mysql_free_result(res);
        return result;
    }
    mysql_free_result(res);
    return 0;
}

int find_usr(MYSQL mysql, int fd, char *id){
    char q_buf[512];
    MYSQL_RES *res;
    MYSQL_ROW row;

    memset(q_buf, 0, 512);
    sprintf(q_buf, "select id from accessor where fd = %d", fd);
    if(mysql_query(&mysql, q_buf)){
        printf("Error %d: %s \n", mysql_errno(&mysql), mysql_error(&mysql));
        exit(1);
    }
    res = mysql_use_result(&mysql);
    row = mysql_fetch_row(res);
    if(row != NULL){
        strcpy(id, row[0]);
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    return 0;
}

int main(int argc, char *argv[]){
    struct sockaddr_in server_addr;
    int sockfd, new_fd; /* listen on sockd, new connection on new_fd */
    struct sockaddr_in cad; /* structure to hold client's address */
    int alen; /* length of address */
    fd_set readfds, activefds; /* the set of read descriptors */
    int i, maxfd = 0, numbytes;
    char buf[100];
    int chat_table[1024] = {0, };
    MYSQL mysql;

    /* Connect database */
    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql, "localhost", "root", "1234", "chat", 0, (char *)NULL, 0)){
        printf("Error %d: %s \n", mysql_errno(&mysql), mysql_error(&mysql));
        exit(1);
    }

    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() failed");
        exit(1);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind (sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind() failed");
        exit(1);
    }
    /* Specify a size of request queue */
    if (listen(sockfd, QLEN) < 0) {
        fprintf(stderr,"listen failed\n");
        exit(1);
    }
    alen = sizeof(cad);
    /* Initialize the set of active sockets. */
    FD_ZERO (&activefds);
    FD_SET (sockfd, &activefds);
    maxfd = sockfd;
    /* Main server loop – accept&handle requests */
    fprintf( stderr, "Server up and running.\n");

    /* start server */
    while (1) {
        printf("SERVER: Waiting for contact ..., %d\n", maxfd);
        /* Block until input arrives on one or more active sockets. */
        readfds = activefds;
        if (select (maxfd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror ("select");
            exit (EXIT_FAILURE);
        }
        /* Service all the sockets with input pending.*/
        for (i = 0; i <= maxfd; ++i) {
            if (FD_ISSET (i, &readfds)) {
                if (i == sockfd) {
                    new_fd=accept(sockfd, (struct sockaddr *)&cad, (&alen));
                    if (new_fd < 0) {
                        fprintf(stderr, "accept failed\n");
                        exit (1);
                    }
                    FD_SET(new_fd, &activefds); //add the new socket desc to our active connections set
                    printf("set %d\n", new_fd);
                    if (new_fd > maxfd)
                        maxfd = new_fd;
                }
                else {
                    char data[1024];
                    char *data_buf;
                    //int cnt = 0;
                    memset(buf, 0, 100);
                    /* Data arriving on an already connected socket. */
                    if ((numbytes=recv (i, buf, 100, 0)) == -1){
                        perror("recv");
                    }

                    /* split data */
                    buf[numbytes]='\0';
                    strcpy(data, buf);
                    data_buf = strtok(buf, "=");

                    if(!strcmp(data_buf, "msg")){   //send msg
                        strcpy(buf, data);
                        /* send data already connected socket. */
                        buf[numbytes]='\0';
                        if (send (chat_table[i], buf, strlen(buf), 0) == -1){
                            perror("send");
                        }
                    }
                    else if(!strcmp(data_buf, "exit")){ //client exit
                        sprintf(buf, "disconn=0");
                        if (send (chat_table[i], buf, strlen(buf), 0) == -1){
                            perror("send");
                        }
                        exit_usr(mysql, i);
                        chat_table[chat_table[i]] = 0;
                        chat_table[i] = 0;
                        close(i);
                        FD_CLR(i, &activefds);
                    }
                    else if(!strcmp(data_buf, "init_usr")) { //init user
                        while(data_buf != NULL) {
                            data_buf = strtok(NULL, "=");
                            if(data_buf != NULL){
                                strcpy(buf, data_buf);
                            }
                        }

                        buf[numbytes]='\0';
                        if(init_usr(mysql, buf, i)){
                            memset(buf, 0, 100);
                            sprintf(buf, "success");
                        }
                        else{
                            memset(buf, 0, 100);
                            sprintf(buf, "failed");
                        }
                        /* send data already connected socket. */
                        if (send (i, buf, strlen(buf), 0) == -1){
                            perror("send");
                        }
                    }
                    else if(!strcmp(data_buf, "enter_usr")) { //enter user
                        while(data_buf != NULL) {
                            data_buf = strtok(NULL, "=");
                            if(data_buf != NULL){
                                strcpy(buf, data_buf);
                            }
                        }

                        buf[numbytes]='\0';
                        if(enter_usr(mysql, buf, i)){
                            memset(buf, 0, 100);
                            sprintf(buf, "success");
                        }
                        else{
                            memset(buf, 0, 100);
                            sprintf(buf, "failed");
                        }
                        /* send data already connected socket. */
                        if (send (i, buf, strlen(buf), 0) == -1){
                            perror("send");
                        }
                    }
                    else if(!strcmp(data_buf, "conn")){    //create chat
                        while(data_buf != NULL) {
                            data_buf = strtok(NULL, "=");
                            if(data_buf != NULL){
                                strcpy(buf, data_buf);
                            }
                        }

                        buf[numbytes]='\0';
                        int target_fd = create_chat(mysql, buf);
                        if(target_fd > 0){
                            char usr_id[128];
                            memset(usr_id, 0, 128);
                            find_usr(mysql, i, usr_id);
                            chat_table[i] = target_fd;
                            chat_table[target_fd] = i;
                            memset(buf, 0, 100);
                            sprintf(buf, "success");
                            /* send data already connected socket. */
                            if (send (i, buf, strlen(buf), 0) == -1){
                                perror("send");
                            }
                            memset(buf, 0, 100);
                            sprintf(buf, "success=%s", usr_id);

                            //send to partner
                            /* send data already connected socket. */
                            if (send (chat_table[i], buf, strlen(buf), 0) == -1){
                                perror("send");
                            }
                        }
                        else{
                            memset(buf, 0, 100);
                            sprintf(buf, "failed");
                            /* send data already connected socket. */
                            if (send (i, buf, strlen(buf), 0) == -1){
                                perror("send");
                            }
                        }
                    }
                    else if(!strcmp(data_buf, "file")){     //send file
                        int size = 0;
                        char f[5000];
                        /* send data already connected socket. */
                        if (send (chat_table[i], data, strlen(data), 0) == -1){
                            perror("send");
                        }
                        while(1){
                            memset(f, 0, 5000);
                            if ((numbytes=recv (i, f, 5000, 0)) == -1){
                                perror("recv");
                            }
                            if (send (chat_table[i], f, strlen(f), 0) == -1){
                                perror("file send");
                                //free(f);
                            }
                            f[numbytes] = '\0';
                            if(!strcmp(f, "finish")){
                                break;
                            }
                            recv (chat_table[i], f, strlen(f), 0);
                            send (i, f, strlen(f), 0);
                        }
                    }
                    else{
                        memset(buf, 0, 100);
                        sprintf(buf, "failed_type=type");
                        if (send (chat_table[i], buf, strlen(buf), 0) == -1){
                            perror("failed type");
                        }
                    }
                }   //end already connected socket
            }
        }   //end loop
    }   //end server
    close(sockfd);
    mysql_close(&mysql);
    return 0;
}