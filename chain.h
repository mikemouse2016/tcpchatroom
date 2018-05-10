//
// Created by Administrator on 2018/4/4.
//

#ifndef MIKE_TEST_H
#define MIKE_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

typedef struct Node{
    int num;
    struct Node *next;
}Node;

typedef struct FindNode{
    int length;
    Node **next;
}FindNode;

Node * appendNode(Node *head,int num);
Node * insertHead(Node *head,int num);
Node * insertNode_before(Node *head,int num,int newnum);
Node * insertNode_after(Node *head,int num,int newnum);
void showNodes(Node *head);
void showNodes_recursion(Node *head);
Node * reverseNode(Node *head);
Node * searchNode(Node *head,int num);
FindNode * searchAllNodes(Node *head,int num);
void showFindNodes(FindNode *findNode);
Node * changeNode(Node *head,int num,int newnum);
Node * changeAllNodes(Node *head,int num,int newnum);
Node * deleteNode(Node *head,int num);
Node * sortNode_bubble(Node *head,char ch);
Node * sortNode_select(Node *head,char ch);
Node * sortNode_fast(Node *head,Node *end,char ch);
int getNode_number(Node *head);
Node *mergeNode(Node *head);
Node * getMidNode(Node *Whead);


#endif //MIKE_TEST_H
