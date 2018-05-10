#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXLINE 4096
#define SERV_PORT 8080
#define LOC_SERVER_ADDR "127.0.0.1" // 本地(默认)
#define ALI_SERVER_ADDR "111.111.111.111" // 阿里云
#define TEN_SERVER_ADDR "111.111.111.111" // 腾讯云
#define MY_IP "111.111.111.111" // 局域网


void *pthread_write(void *arg) {
    int conn = *(int *)arg;
    int length = 0;
    char sendBuf[MAXLINE] ={0};
    while (1) {
        setbuf(stdin,NULL);
        fgets(sendBuf,MAXLINE-1,stdin);
        *(sendBuf+strlen(sendBuf)-1) ='\0';
        length = write(conn, sendBuf, strlen(sendBuf)+1);
        if (length == -1) {
            perror("send to server error\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;   //服务器网络地址和端口
    memset(&server_addr,0,sizeof(server_addr));  // 置字节字符串前n个字节为零且包括‘\0’
    server_addr.sin_family= AF_INET; // tcp.udp网络通信协议
    server_addr.sin_addr.s_addr=inet_addr(LOC_SERVER_ADDR);//服务器IP地址
    server_addr.sin_port= htons(SERV_PORT);  //服务器端口号

    // 创建客户端套接字--IPv4协议,失败返回-1
    int client_sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(client_sockfd ==-1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // 用于建立与指定socket的连接
    if (connect(client_sockfd,(struct sockaddr *)&server_addr, sizeof(server_addr))==-1){
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    char buf[MAXLINE] ={0};
    int len;
    pthread_t pthread_main;

    if ((pthread_create(&pthread_main, NULL, pthread_write,&client_sockfd)) == -1){
        perror("server pthread_create error");
        exit(EXIT_FAILURE);
    }
    pthread_detach(pthread_main); // 线程分离，结束时自动回收资源

    for (;;) {
        memset(buf,0,MAXLINE);
        len = read(client_sockfd, buf, MAXLINE);
        if(len == 0){
            printf("the other side has been closed.\n");
            break;
        }
        if(strstr(buf,"over")){
            printf("123");
            break;
        }
        write(STDOUT_FILENO,buf,len);
        write(STDOUT_FILENO,"\n",strlen("\n"));
    }

    close(client_sockfd);
    return 0;
}
