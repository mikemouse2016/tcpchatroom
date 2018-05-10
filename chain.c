//
// Created by Administrator on 2018/4/4.
//
#include "chain.h"

// 尾部添加节点
Node * appendNode(Node *head,int num){
    Node *newNode = malloc(sizeof(Node));
    newNode->num=num;
    newNode->next=NULL;
    if (head==NULL){
        return newNode;
    }else{
        Node *p = head;
        while (p->next){
            p=p->next;
        }
        p->next=newNode;
        return head;
    }
}

// 头部添加节点
Node * insertHead(Node *head,int num){
    Node *newNode = malloc(sizeof(Node));
    newNode->num=num;
    if (head==NULL){
        newNode->next=NULL;
        return newNode;
    }else{
        Node *p = head;
        newNode->next=head;
        return newNode;
    }
}

// 前面插入节点
Node * insertNode_before(Node *head,int num,int newnum){
    Node *newNode =malloc(sizeof(Node));
    newNode->num=newnum;
    if (head==NULL){
        newNode->next=NULL;
        return newNode;
    }
    if (head->num==num){
        newNode->next=head;
        return newNode;
    }else{
        Node *p = head;
        while (p->next){
            if (p->next->num==num){
                newNode->next=p->next;
                p->next=newNode;
                break;
            }
            p=p->next;
        }
        return head;
    }
}

// 后面插入节点
Node * insertNode_after(Node *head,int num,int newnum){
    Node *newNode =malloc(sizeof(Node));
    newNode->num=newnum;
    if (head==NULL){
        newNode->next=NULL;
        return newNode;
    }
    if (head->num==num){
        newNode->next=head->next;
        head->next=newNode;
        return head;
    }else{
        Node *p = head;
        while (p->next){
            if (p->next->num==num){
                newNode->next=p->next->next;
                p->next->next=newNode;
                break;
            }
            p=p->next;
        }
        return head;
    }
}

void showNodes(Node *head){
    while (head){
        printf("节点:%d\n",head->num);
        fflush(stdout);
        head=head->next;
    }
}

void showNodes_recursion(Node *head){
    if (head==NULL){
        return;
    }
    printf("%d ",head->num);
    showNodes_recursion(head->next);
}

// 链表反转
Node * reverseNode(Node *head){
    if (head==NULL||head->next==NULL){
        return head;
    }
    Node *p1 = head;
    Node *p2 = p1->next;
    Node *p3;
    while (p2){
        p3=p2->next;
        p2->next=p1;
        p1=p2;
        p2=p3;
    }
    head->next=NULL;
    return p1;
}

// 递归实现链表反转
Node * reverseNode_recursion(Node *head){
    if (head==NULL||head->next==NULL){
        return head;
    }
    Node *pnext = head->next; // 顺序
    Node *p = reverseNode_recursion(pnext); // 返回值永远不变，永远返回最后一个节点
    pnext->next=head;// 逆序
    head->next=NULL; // 逆序
    return p;
}

//  查找第一个符合条件的节点
Node * searchNode(Node *head,int num){
    if (head==NULL){
        return head;
    }
    while (head){
        if (head->num==num){
            return head;
        }
        head=head->next;
    }
    return NULL;
}

// 查找所有符合条件的节点
FindNode * searchAllNodes(Node *head,int num){
    if (head==NULL){
        return NULL;
    }
    int flag =0;
    FindNode *findNode = malloc(sizeof(FindNode));
    Node *p = head;
    findNode->length =0;
    while (p){
        if (p->num==num){
            flag=1;
            findNode->length++;
            if (p==head){
                findNode->next=malloc(sizeof(Node *)*findNode->length);
                findNode->next[0]=p;
            } else {
                findNode->next = realloc(findNode->next, sizeof(Node *) * findNode->length);
                findNode->next[findNode->length - 1]=p;
            }
        }
        p=p->next;
    }
    if (!flag){
        return NULL;
    }else{
        return findNode;
    }
}

void showFindNodes(FindNode *findNode){
    for (int i = 0; i < findNode->length; ++i) {
        printf("地址:%p,数据:%d\n",findNode->next[i],findNode->next[i]->num);
    }
}

//  修改第一个符合条件的节点
Node * changeNode(Node *head,int num,int newnum){
    if (head==NULL){
        return head;
    }
    while (head){
        if (head->num==num){
            head->num=newnum;
            return head;
        }
        head=head->next;
    }
    return NULL;
}

// 修改所有符合条件的节点
Node * changeAllNodes(Node *head,int num,int newnum){
    if (head==NULL){
        return head;
    }
    int flag =0;
    Node *p = head;
    while (p){
        if (p->num==num){
            flag=1;
            p->num=newnum;
        }
        p=p->next;
    }
    if (!flag){
        return NULL;
    }else{
        return head;
    }
}

// 删除第一个符合条件的节点
Node * deleteNode(Node *head,int num){
    if (head==NULL){
        return head;
    }
    Node *p1,*p2;
    p1=head;
    if (p1->num==num){
        p2=p1->next;
        free(p1);
        return p2;
    }else{
        while (p1->next){
            p2=p1->next;
            if (p2->num==num){
                p1->next=p2->next;
                free(p2);
                break;
            }else{
                p1=p1->next;
            }
        }
        return head;
    }
}

// 删除所有符合条件的节点
Node * deleteAllNodes(Node *head,int num){
    if (head==NULL){
        return head;
    }
    Node *p1,*p2,*p3;
    p3=p1=head;
    while (p1->next){
        p2=p1->next;
        if (p2->num==num){
            p1->next=p2->next;
            free(p2);
        }else{
            p1=p1->next;
        }
    }
    if (p3->num==num){
        head=head->next;
        free(p3);
    }
    return head;
}

// 冒泡排序
Node * sortNode_bubble(Node *head,char ch){
    if (ch=='<'){
        for (Node *p1=head;p1;p1=p1->next){
            for (Node *p2=head;p2;p2=p2->next){
                if (p1->num<p2->num){
                    int temp = p1->num;
                    p1->num=p2->num;
                    p2->num=temp;
                }
            }
        }
    }else if (ch == '>'){
        for (Node *p1=head;p1;p1=p1->next){
            for (Node *p2=head;p2;p2=p2->next){
                if (p1->num>p2->num){
                    int temp = p1->num;
                    p1->num=p2->num;
                    p2->num=temp;
                }
            }
        }
    }
}

Node * sortNode_select(Node *head,char ch){
    if (ch=='<'){
        for (Node *p1=head;p1;p1=p1->next){
            Node *min= p1;
            for (Node *p2=head->next;p2;p2=p2->next){
               if (p2->num<min->num){
                   min=p2;
               }
            }
            if (min!=p1){
                int temp=p1->num;
                p1->num=min->num;
                min->num=temp;
            }
        }
    }else if (ch == '>'){
        for (Node *p1=head;p1;p1=p1->next){
            Node *min= p1;
            for (Node *p2=head->next;p2;p2=p2->next){
                if (p2->num>min->num){
                    min=p2;
                }
            }
            if (min!=p1){
                int temp=p1->num;
                p1->num=min->num;
                min->num=temp;
            }
        }
    }
}

// 数组法
void sort_array_realize(int *p,int start,int end,char ch){
    if (start>=end){
        return;
    }
    int i=start;
    int j=i+1;
    while (j<=end){
        if (ch == '<'){
            if (p[j]<p[start]){
                i++;
                int temp = p[i];
                p[i]=p[j];
                p[j]=temp;
            }
        }else if (ch=='>'){
            if (p[j]>p[start]){
                i++;
                int temp = p[i];
                p[i]=p[j];
                p[j]=temp;
            }
        }
        j++;
    }
    int temp = p[i];
    p[i]=p[start];
    p[start]=temp;
    sort_array_realize(p,start,i-1,ch);
    sort_array_realize(p,i+1,end,ch);
}

void sort_array(){
    int a[10] = {1,5,2,7,2,5,7,8,0,2};
    sort_array_realize(a,0,9,'<');
    for (int i = 0; i < 10; ++i) {
        printf("%d ",a[i]);
    }
}

// 原理，把小于第一个数的值和都移动左边，大于第一个数的值都移动到右边
// p2起到找到最左边第一个小于10的数
void sortNode_fast_realize(Node *head,Node *end, char ch){
    if (head==end){
        return;
    }
    Node *p1 = head;
    Node *p2 = p1->next;
    int key = p1->num;
    while (p2!=end){
        if (ch=='<'){
            if (p2->num<key){
                p1=p1->next;
                int temp =p1->num;
                p1->num=p2->num;
                p2->num=temp;
            }
        } else if (ch=='>'){
            if (p2->num>key){
                p1=p1->next;
                int temp =p1->num;
                p1->num=p2->num;
                p2->num=temp;
            }
        }
        p2=p2->next;
       // showNodes_recursion(head);
       // printf("\n");
    }
    int temp = head->num;
    head->num=p1->num;
    p1->num=temp;
    //printf("进入循环\n");
    sortNode_fast_realize(head,p1,ch);
    sortNode_fast_realize(p1->next,end,ch);
}

// 快速排序
Node * sortNode_fast(Node *head,Node *end,char ch){
    if (ch == '<'){
        sortNode_fast_realize(head,end,'<');
    }else if (ch == '>'){
        sortNode_fast_realize(head,end,'>');
    }
}

// 获取链表个数
int getNode_number(Node *head){
    int length =0;
    while (head){
        length++;
        head=head->next;
    }
    return length;
}

int getNode_number_recursion(Node *head){
    if (head==NULL){
        return 0;
    }
    return 1+getNode_number_recursion(head->next);
}

// 链表和并
Node *mergeNode(Node *head){
    Node *head1 = NULL;
    head1 = appendNode(head1,1);
    head1 = appendNode(head1,3);
    head1 = appendNode(head1,5);
    head1 = appendNode(head1,7);
    head1 = appendNode(head1,9);
    head1 = appendNode(head1,11);
    head1 = appendNode(head1,13);


    Node *head2 = NULL;
    head2 = appendNode(head2,2);
    head2 = appendNode(head2,4);
    head2 = appendNode(head2,6);
    head2 = appendNode(head2,8);


    while (head1!=NULL&&head2!=NULL){
        if (head1->num<head2->num){
            head = appendNode(head,head1->num);
            head1=head1->next;
        }else{
            head = appendNode(head,head2->num);
            head2=head2->next;
        }
    }
    if (head1==NULL){
        while (head2){
            head = appendNode(head,head2->num);
            head2=head2->next;
        }
    }else{
        while (head1){
            head = appendNode(head,head1->num);
            head1=head1->next;
        }
    }
    return head;
}

// 链表取中间值
Node * getMidNode(Node *head){
    if (head==NULL||head->next==NULL){
        return head;
    }
    Node *p1 =head;
    Node *p2 =head;
    while (p2->next){
        p1=p1->next; // p1前进1步
        p2=p2->next; // p2前进2步
        if (p2->next){
            p2=p2->next;
        }
    }
    return p1;
}

// 环形链表
Node * circularNode(Node *head){
    head = appendNode(head,1);
    head = appendNode(head,2);
    head = appendNode(head,2);
    head = appendNode(head,3);
    head = appendNode(head,6);
    Node *p = head;
    while (p->next){
        p=p->next;
    }
    p->next=head;
    return head;
}

// 显示环形链表
void showcircularNodes(Node *head){
    Node *p=head;
    while (p->next!=head){
        printf("%d ",p->num);
        p=p->next;
    }
    printf("%d",p->num);
}

// 判断链表是否有环
int iscircularNode(Node *head){
    if (head==NULL){
        return 0;
    }
    Node *p1 =head;
    Node *p2 =head;
    int flag =0;
    while (p2){
        if (p2->next){
            p2=p2->next;
        }
        if (p1==p2){
            flag=1;
            break;
        }
        p1=p1->next; // p1前进1步
        p2=p2->next; // p2前进2步
    }
    return flag;
}

void main1(){
    Node *head = NULL;
    printf("尾部插入\n");
    head = appendNode(head,1);
    head = appendNode(head,2);
    head = appendNode(head,2);
    head = appendNode(head,3);
    head = appendNode(head,6);
    showNodes_recursion(head);

    printf("\n头部插入\n");
    head = insertHead(head,4);
    showNodes_recursion(head);

    printf("\n前面插入\n");
    head = insertNode_before(head,6,5);
    showNodes_recursion(head);

    printf("\n后面插入\n");
    head = insertNode_after(head,4,7);
    showNodes_recursion(head);

    printf("\n链表反转\n");
    head = reverseNode(head);
    showNodes_recursion(head);
    printf("\n");
    head = reverseNode_recursion(head);
    showNodes_recursion(head);

    printf("\n查找符合条件的所有节点\n");
    FindNode *findNodes = searchAllNodes(head,2);
    if (findNodes) {
        showFindNodes(findNodes);
    }

    printf("修改第一个节点\n");
    Node *pfind = changeNode(head,2,22);
    showNodes_recursion(head);

    printf("\n修改符合条件的所有节点\n");
    head = changeAllNodes(head,2,33);
    showNodes_recursion(head);

    printf("\n删除第一个符合条件的节点\n");
    head = deleteNode(head,3);
    showNodes_recursion(head);

    printf("\n删除符合条件的所有节点\n");
    head = deleteAllNodes(head,33);
    showNodes_recursion(head);

    printf("\n冒泡排序\n");
    sortNode_bubble(head,'<');
    showNodes_recursion(head);

    printf("\n选择排序\n");
    sortNode_bubble(head,'<');
    showNodes_recursion(head);

    printf("\n数组快速排序\n");
    sort_array();

    printf("\n快速排序\n");
    sortNode_fast(head,NULL,'<');
    showNodes_recursion(head);

    printf("\n节点个数\n");
    int number = getNode_number(head);
    printf("%d",number);
    int number1 = getNode_number_recursion(head);
    printf("%d",number1);

    printf("\n链表合并\n");
    Node *head1=NULL;
    head1 = mergeNode(head1);
    showNodes_recursion(head1);

    printf("\n链表取中间值\n");
    Node *mid = getMidNode(head);
    printf("%d",mid->num);

    printf("\n环形链表\n");
    Node *head2 = NULL;
    head2 =circularNode(head2);
    showcircularNodes(head2);

    if (iscircularNode(head)){
        printf("\n是环形链表\n");
    }else{
        printf("\n不是环形链表\n");
    }
}
