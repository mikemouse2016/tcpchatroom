#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio_ext.h>
#include "chain.h"


/*
 * 如果用一个conn变量的情况下的问题
 * 假如有多个客户端要连接这个服务器，正常的情况下，一个客户端连接对应一个 connfd，相互之间独立不受影响，
 * 但是，假如多个客户端同时连接这个服务器，A 客户端的连接套接字为 connfd，服务器正在用这个 connfd 处理数据，
 * 还没有处理完，突然来了一个 B 客户端，accept()之后又生成一个 connfd, 因为是地址传递，
 * A 客户端的连接套接字也变成 B 这个了，这样的话，服务器肯定不能再为 A 客户端服务器了
 * 只能用锁，锁住,accpept,创建线程,解锁，循环接受消息
 */

#define MAXLINE 4096
#define SERV_PORT 8080
#define QUEUELEN 10  // 同时在线最大客户数
#define LOC_SERVER_ADDR "127.0.0.1" // 本地(默认)
#define ALI_SERVER_ADDR "111.111.111.111" // 阿里云
#define TEN_SERVER_ADDR "111.111.111.111" // 腾讯云
#define MY_IP "111.111.111.111" // 局域网

Node * head = NULL; // 队列

typedef struct Client{
    pthread_t pthreadReadID;     // 线程ID
    int clientID;               // 客户ID
    char ip[30];                // ip地址
    char port[10];              // 端口号
    int  client_sockfd;  	     // accept的返回的客户端的新的套接字描述符
    int isOnline;               // 是否上线,1上线，0没有上线
}Client;

Client client[QUEUELEN] ={0};
char sendBuf[MAXLINE] ={0};
pthread_mutex_t mutex;
pthread_cond_t cond;


void cleanup(void *arg) {
    //   printf("线程%s释放\n", (char *)arg);
}

/*
1，线程放在等待队列上，解锁
2，等待 pthread_cond_signal或者pthread_cond_broadcast信号之后去竞争锁
3，若竞争到互斥索则加锁。
 */

void * pthread_write(void *pVoid) {
    pthread_mutex_lock(&mutex);
    int length = 0;
    while (1){
      //  showNodes(head);
        pthread_cond_wait(&cond,&mutex);
        for (int i = 0; i < QUEUELEN; ++i) {
            Node *find = searchNode(head, client[i].client_sockfd);
           // printf("i:%d的客户套接字:%d\n",i,client[i].client_sockfd);
            if (find != NULL && strcmp(client[i].ip,"")!=0 && strcmp(client[i].port,"")!=0) {
            //    printf("要发送的客户套接字:%d\n",client[i].client_sockfd);
                length = (int) write(client[i].client_sockfd, sendBuf, strlen(sendBuf) + 1);
                if (length == -1) {
                    perror("send to client error");
                }
            }
        }
    }
}


void *pthread_read(void *arg) {
    Client *client = (Client *)arg;
    int length = 0;
    char buf[MAXLINE] ={0};
    memset(sendBuf,0,MAXLINE);
    while (1) {
        memset(buf,0,MAXLINE);
        length = (int)read(client->client_sockfd, buf, MAXLINE);
        char finalBuf[MAXLINE] ={0};
        if (length == -1 || length == 0) {
            sprintf(finalBuf,"%s %s:exit",client->ip,client->port);
            write(STDOUT_FILENO,finalBuf,strlen(finalBuf)+1);
            write(STDOUT_FILENO,"\n",strlen("\n"));
            strcpy(sendBuf,finalBuf);
            head = deleteNode(head,client->client_sockfd);
          //  pthread_write();
            pthread_cond_signal(&cond);
            printf("退出后当前链接人数:%d\n",getNode_number(head));
            break;
        }
        sprintf(finalBuf,"%s %s:%s",client->ip,client->port,buf);
      //  write(STDOUT_FILENO,"服务器读取:",strlen("服务器读取:")+1);
        write(STDOUT_FILENO,finalBuf,strlen(finalBuf)+1);
        write(STDOUT_FILENO,"\n",strlen("\n"));
        strcpy(sendBuf,finalBuf);
       // pthread_write();
        pthread_cond_signal(&cond);
    }
    close(client->client_sockfd);
    memset(client, 0, sizeof(Client)); // 必须要把该套接字清空
    // 线程退出需要释放自身资源
    pthread_cleanup_push(cleanup,NULL);
    if (!arg){
        pthread_exit(0);
    }
    pthread_cleanup_pop(0);
}


void * accept_client(void *pVoid) {
    struct sockaddr_in server_addr;   //服务器网络地址和端口
    memset(&server_addr, 0, sizeof(server_addr)); // 置字节字符串前n个字节为零且包括‘\0’
    server_addr.sin_family = AF_INET; // tcp.udp网络通信协议 m
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//服务器IP地址
    server_addr.sin_port = htons(SERV_PORT);  //服务器端口号

    // 创建客户端套接字--IPv4协议,失败返回-1
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd == -1) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    // 将套接字绑定到服务器的网络地址上
    if (bind(server_sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind error");
        exit(EXIT_FAILURE);
    }

    // 监听服务器端套接字，1000代表最多有1000个人链接
    if (listen(server_sockfd, 1000) == -1) {
        perror("listen error");
        exit(EXIT_FAILURE);
    };

    struct sockaddr_in client_addr;   //客户端网络地址和端口
    socklen_t clientaddr_len = sizeof(client_addr);
    char buf[MAXLINE] = {0}; //数据传送的缓冲区
    char str[INET_ADDRSTRLEN]; // 存放ip地址
    int len;
    int conn;

    for (int i = 0; ; ++i) {
        conn = accept(server_sockfd, (struct sockaddr *) &client_addr, &clientaddr_len);

        head = appendNode(head,conn);
        if (getNode_number(head)>QUEUELEN){
            char str1[128] ="超过最大链接人数\n";
            write(conn, str1, strlen(str1)+1);
            close(conn);
            head = deleteNode(head,conn);
            continue;
        }else{
            printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str)), ntohs(client_addr.sin_port));
            printf("进来后当前链接人数:%d\n",getNode_number(head));

            // 把被清零的第一个客户重新分配新的套接字，套接字有可能为0，所以必须考虑0的情况
            for (i = 0; i < QUEUELEN; i++) {
                if (client[i].client_sockfd == 0 && strcmp(client[i].ip,"")==0 && strcmp(client[i].port,"")==0) {
                    client[i].client_sockfd = conn;
                    break;
                }
            }
            client[i].client_sockfd = conn;
            strcpy(client[i].ip ,str);
            sprintf(client[i].port,"%d",ntohs(client_addr.sin_port));
            client[i].clientID =i;
            client[i].isOnline =1;
            memset(sendBuf,0,MAXLINE);
            sprintf(sendBuf,"%s %s:%s",client[i].ip,client[i].port,"come in");
            write(STDOUT_FILENO,sendBuf,strlen(sendBuf)+1);
            write(STDOUT_FILENO,"\n",strlen("\n"));
            pthread_cond_signal(&cond);
        }

        if ((pthread_create(&client[i].pthreadReadID, NULL, pthread_read,&client[i])) == -1){
            perror("server pthread_read error");
            exit(EXIT_FAILURE);
        }
        pthread_detach(client[i].pthreadReadID); // 线程分离，结束时自动回收资源
    }
}

void main(){
    pthread_t pthread_main_t;
    pthread_t pthread_write_t;
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);
    pthread_create(&pthread_main_t,NULL,accept_client,sendBuf);
    pthread_detach(pthread_main_t);

    if ((pthread_create(&pthread_write_t, NULL, pthread_write,NULL)) == -1){
        perror("server pthread_read error");
        exit(EXIT_FAILURE);
    }
    pthread_detach(pthread_write_t); // 线程分离，结束时自动回收资源

    char buf[MAXLINE]={0};
    while (1) {
        setbuf(stdin,NULL);
        fgets(buf,MAXLINE-1,stdin);
        *(buf+strlen(buf)-1) ='\0';
        sprintf(sendBuf,"%s %d:%s",LOC_SERVER_ADDR,SERV_PORT,buf);
       // pthread_write();
        pthread_cond_signal(&cond);
    }
}