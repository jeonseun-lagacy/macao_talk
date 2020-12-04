# macao_talk
전세운, 송민광 시스템 프로그래밍 기말 팀 과제
> macaoTalkClient 폴더에 download 폴더를 만들어주어야 Client가 정상적으로 동작 합니다.

* 소켓,  DB, QT를 이용

## 1. 개요
> 채팅방 구성과 파일전송이 가능한 GUI 메신저 프로그램을 구현한다. 스레드와 소켓, 데이터베이스 GUI를 이용하며 FTP 프로토콜을 이용하여 파일전송 기능을 구현한다.
## 2. 설계 내용
>### 1. 프로그램 구조도
>> <img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/structure.png?raw=true" title="프로그램 구조도"></img><br/>
>### 2. 서버 역할
>> 데이터베이스에 직접 접속하여 데이터를 입출력하고 클라이언트 간 채팅 및 파일전송 기능의 허브 역할
>### 3. 데이터베이스 역할
>> 이용자 정보를 저장하고 해당 이용자의 소켓 정보를 저장
>>
>> <img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/db.png?raw=true" title="데이터베이스"></img><br/>
>### 4. 클라이언트 역할
>> 사용자 등록을 위해 GUI를 통해 값을 입력받아 서버에 전송하며 채팅방 생성을 위해 채팅 상대방을 입력받아 채팅방을 화면에 띄우고 서버와의 소켓 통신을 통해 채팅과 파일전송 기능 제공
## 3. 구현 내용
>### 1.사용자 등록
>> |최초 화면|등록 실패(이미 있는 이름)|등록 성공|
>> |:----:|:----:|:----:|
>> |<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/login.png?raw=true" title="로그인 창"></img>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/login_failed.png?raw=true" title="로그인 실패"></img>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/login_success.png?raw=true" title="로그인 성공"></img>|
server(/macaoTalkServer/main.c)의 구현 부
<pre><code>int init_usr(MYSQL mysql, char *arg, int fd){   //init user (success = 1, failed = 0)
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
</code></pre>
>> 테이블의 id 필드를 모두 조회해서 Client에서 입력받은 값과 일치하는 값이 없을 경우 해당 id를 테이블에 추가한다.
>### 2. 로그인
>>|로그인 화면|메인화면|
>>|:---:|:---:|
>>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/login.png?raw=true" title="로그인"></img>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/main.png?raw=true" title="메인 화면"></img>|
server(/macaoTalkServer/main.c)의 구현 부
<pre><code>int enter_usr(MYSQL mysql, char *arg, int fd){ //login user (success = 1, failed = 0)
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
</code></pre>
>> Client에서 입력받은 값과 일치하는 id를 조건으로 조회한 데이터가 존재하면 해당 행의 fd필드를 현재 Client가 접속중인 소켓의 File Descriptor로 update 한다.
>### 3. 채팅방 생성
>>|메인 화면|채팅 상대의 이름 입력|채팅 생성|
>>|:---------:|:---------:|:---------:|
>>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/main.png?raw=true" title="메인화면"></img>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/connecting.png?raw=true" title="이름입력"></img>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/chat.png?raw=true" title="채팅생성"></img>|
server(/macaoTalkServer/main.c)의 구현 부
<pre><code>int create_chat(MYSQL mysql, char * arg){  //create chat (return target fd)
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
    if(row !=NULL){
        int result = atoi(row[0]);
        mysql_free_result(res);
        return result;
    }
    mysql_free_result(res);
    return 0;
}
.
.
.
/*main의 받은 메시지 처리 부*/
else if(!strcmp(data_buf, "conn")){    //create chat
    while(data_buf !=NULL) {
        data_buf = strtok(NULL, "=");
        if(data_buf !=NULL){
            strcpy(buf, data_buf);
        }
    }
    buf[numbytes]='\0';
    int target_fd = create_chat(mysql, buf);
    if(target_fd >0){
        char usr_id[128];
        memset(usr_id, 0, 128);
        find_usr(mysql, i, usr_id);
        chat_table[i] = target_fd;
        chat_table[target_fd] = i;
        memset(buf, 0, 100);
        sprintf(buf, "success");
        /* send data already connected socket. */
        if (send (i, buf, strlen(buf), 0) ==-1){
            perror("send");
        }
        memset(buf, 0, 100);
        sprintf(buf, "success=%s", usr_id);
        //send to partner
        /* send data already connected socket. */
        if (send (chat_table[i], buf, strlen(buf), 0) ==-1){
            perror("send");
        }
    }
    else{
        memset(buf, 0, 100);
        sprintf(buf, "failed");
        /* send data already connected socket. */
        if (send (i, buf, strlen(buf), 0) ==-1){
            perror("send");
        }
    }
}
</code></pre>
>> Client에서 입력받은 값과 일치하는 id를 조건으로 fd필드를 조회하여 반환한다.
   반환 값이 0일 경우 Client에 “failed”를 전송한다.
   반환 값이 0이 아닐 경우 “success”를 전송하고, 해당 반환 값 소켓에 “success=채팅요청한 id”를 전송한다. 그리고 채팅 요청한 Client의 fd를 인덱스로 하여 배열의 해당 인덱스에 상대 fd를 저장하고, 상대 인덱스에는 요청한 Client의 fd를 저장한다.
> ### 4. 파일 전송
>> |파일 선택 화면|수신 결과|수신자 채팅방|송신자 채팅방|
>> |:------:|:------:|:------:|:------:|
>> |<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/select_file.png?raw=true" title="파일 선택"></img>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/file_result.png?raw=true" title="수신 결과"></img>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/get_result.png?raw=true" title="수신자 채팅방"></img>|<img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/send_result.png?raw=true" title="송신자 채팅방"></img>|
client(/macaoTalkClient/mainwindow.cpp)의 구현 부(송신자)
<pre><code>void MainWindow::on_file_btn_clicked()
{
    QStringList list;
    char name_buf[512];
    char buf[MAXDATASIZE];
    int filesize =5000;
    struct stat obj;
    int filehandle;
    int len;
    int size =0;
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
        size =0;
        size = read(filehandle, file_buf, filesize);
        if (size >0){
            if (send(csock, file_buf, size, 0) !=size)
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
    sprintf(msg_buf, "%s send to %s<", file, this ->partner);
    QString value = msg_buf;
    QLabel *label =new QLabel;
    label->setText(value);
    label->setAlignment(Qt::AlignRight);
    ui->scroll_layout ->addWidget(label);
}
</code></pre>
>>파일을 선택하면 “file=파일이름”으로 메시지를 전송한다. 이후 버퍼의 크기만큼 파일을 읽어 버퍼를 메시지로 하여 전송하고 회신을 받는다. 파일이 끝날 때 까지 반복한다.
  파일이 끝나면 “finish”메시지를 전송한다.

client(/macaoTalkClient/mainwindow.cpp)의 구현 부(수신자)
<pre><code>else if (!strcmp(data_buf, "file")) //get file
{
    int c =0;
    int filesize =5000;
    char f[filesize];
    char dir[2048];
    int filehandle;
    while (data_buf !=NULL)
    {
        data_buf = strtok(NULL, "=");
        if (data_buf !=NULL)
        {
            memset(dir, 0, 2048);
            sprintf(dir, "./download/%s", data_buf);
            filehandle = open(dir, O_WRONLY | O_CREAT | O_TRUNC /**/, 0777);
            while (1) {
                memset(f, 0, filesize);
                if ((numbytes = recv(csock, f, filesize, 0)) ==-1)
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
            sprintf(msg_buf, "%s> %s\nDownload complete!", this ->partner, data_buf);
            QString value = msg_buf;
            QLabel *label =new QLabel;
            label->setText(value);
            label->setAlignment(Qt::AlignLeft);
            ui->scroll_layout ->addWidget(label);
        }
    }
}
</code></pre>
>>받은 메시지를 “=”을 토큰으로 나누어 앞부분이 file일 경우 해당 부분을 시작한다.
   토큰으로 나눈 뒷부분을 이름으로 현재 실행파일과 같은 level에 있는 download 디렉토리에 파일을 생성하여 open한다. 이후 받은 메시지가 “finish”일 때 까지 받은 메시지를 파일에 쓰고 반복한다.
> ### 5. 메시지 채팅
>> 채팅 화면<br/>
>> <img src="https://github.com/alsrhkd77/macao_talk/blob/master/screenshot/chat_message.png?raw=true" title="채팅"></img>

client(/macaoTalkClient/mainwindow.cpp)의 구현 부(송신자)
<pre><code>void MainWindow::on_send_btn_clicked()
{
    char buf[MAXDATASIZE];
    char input[128];
    int len;
    QString str = ui ->send_edit ->text();
    strcpy(input, str.toUtf8());
    memset(buf, 0, MAXDATASIZE);
    sprintf(buf, "msg=%s", input);
    len = strlen(buf);
    if (send(csock, buf, len, 0) != len)
    {
        fprintf(stderr, "send failed...\n");
        exit(1);
    }
    ui->send_edit ->setText("");
    char msg_buf[512];
    memset(msg_buf, 0, 512);
    sprintf(msg_buf, "%s<", input);
    QString value = msg_buf;
    value = value.rightJustified(80, ' ');
    QLabel *label =new QLabel;
    label->setText(value);
    label->setAlignment(Qt::AlignRight);
    ui->scroll_layout ->addWidget(label);
}

</code></pre>
>> GUI의 LineEdit에 입력받은 값을 이용해 ”msg=입력받은 값“ 형식으로 전송한다.

client(/macaoTalkClient/mainwindow.cpp)의 구현 부(수신자)
<pre><code>
else if (!strcmp(data_buf, "msg"))  //get chat msg
{
    while (data_buf !=NULL)
    {
        data_buf = strtok(NULL, "=");
        if (data_buf !=NULL)
        {
            char msg_buf[512];
            memset(msg_buf, 0, 512);
            sprintf(msg_buf, "%s> %s", this ->partner, data_buf);
            QString value = msg_buf;
            QLabel *label =new QLabel;
            label->setText(value);
            label->setAlignment(Qt::AlignLeft);
            ui->scroll_layout ->addWidget(label);
        }
    }
}

</code></pre>
>>
>> 받은 메시지를 “=”을 토큰으로 나누어 앞부분이 msg일 경우 토큰 뒤의 내용을 이용해 Label을 생성하여 채팅 화면에 레이아웃에 추가해준다.
> ### 6. Qt 소켓 통신
client(/macaoTalkClient/main.cpp)의 구현 부
<pre><code>
int main(int argc, char *argv[])
{
    int csock;
    struct sockaddr_in serv_addr;
    /* 클라이언트 소켓 생성 */
    if ((csock = socket(AF_INET, SOCK_STREAM, 0)) ==-1)
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
                sizeof(struct sockaddr)) ==-1)
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

</code></pre>
>> 프로그램이 시작되면 바로 서버에 연결할 수 있게 첫 window 객체 생성 전에 서버에 연결하고 window의 생성자에 소켓 fd를 주어 생성된 window에서 바로 통신을 처리할 수 있게 한다.

client(/macaoTalkClient/mainwindow.h)의 구현 부
<pre><code>
class MainWindow : public QMainWindow
{
.
.
.
private slots:
    void wait_chat();
.
.
.
};

</code></pre>
>> 메시지 수신하면서 메시지를 전송할 수 있도록 메시지를 수신하고 처리하는 함수인 wait_chat() 함수를 slot에 선언해준다.

client(/macaoTalkClient/mainwindow.cpp)의 구현 부
<pre><code>
#define MAXDATASIZE 128
MainWindow::MainWindow(int csock, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this ->csock = csock;
    socnot =new QSocketNotifier(csock, QSocketNotifier::Read, this);
    connect(socnot, SIGNAL( activated(int)), this, SLOT( wait_chat()));
    ui->scrollArea ->setWidgetResizable(true);
}
.
.
.
void MainWindow::wait_chat(){
    char buf[MAXDATASIZE];
    char *data_buf;
    int numbytes;
    int size =0;
    memset(buf, 0, MAXDATASIZE);
    if ((numbytes = recv(csock, buf, MAXDATASIZE, 0)) ==-1)
    {
        perror("recv");
        exit(1);
    }
    buf[numbytes] ='\0';
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
        while (data_buf !=NULL)
        {
            data_buf = strtok(NULL, "=");
            if (data_buf !=NULL)
            {
                strcpy(this ->partner, data_buf);
                setChatMode();
            }
        }
    }
    else if (!strcmp(data_buf, "msg"))  //get chat msg
    {
        while (data_buf !=NULL)
        {
            data_buf = strtok(NULL, "=");
            if (data_buf !=NULL)
            {
                char msg_buf[512];
                memset(msg_buf, 0, 512);
                sprintf(msg_buf, "%s> %s", this ->partner, data_buf);
                QString value = msg_buf;
                QLabel *label =new QLabel;
                label->setText(value);
                label->setAlignment(Qt::AlignLeft);
                ui->scroll_layout ->addWidget(label);
            }
        }
    }
    else if (!strcmp(data_buf, "file")) //get file
    {
        int c =0;
        int filesize =5000;
        char f[filesize];
        char dir[2048];
        int filehandle;
        while (data_buf !=NULL)
        {
            data_buf = strtok(NULL, "=");
            if (data_buf !=NULL)
            {
                memset(dir, 0, 2048);
                sprintf(dir, "./download/%s", data_buf);
                filehandle = open(dir, O_WRONLY | O_CREAT | O_TRUNC /**/, 0777);
                while (1) {
                    memset(f, 0, filesize);
                    if ((numbytes = recv(csock, f, filesize, 0)) ==-1)
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
                sprintf(msg_buf, "%s> %s\nDownload complete!", this ->partner, data_buf);
                QString value = msg_buf;
                QLabel *label =new QLabel;
                label->setText(value);
                label->setAlignment(Qt::AlignLeft);
                ui->scroll_layout ->addWidget(label);
            }
        }
    }
}
</code></pre>
>>
>> socket에 메시지가 들어왔을 때 wait_chat() 함수가 호출될 수 있도록 QSocketNotifier 객체를 Read 모드로 생성한 후 wait_chat() 함수와 연결해준다.
   wait_chat() 함수에서는 받은 메시지를 “=”을 토큰으로 나누어 앞부분을 기준으로 정의된 행동을 실행한다.
>
