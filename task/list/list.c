#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;          // 数据域
    struct Node *next; //指针域
};

// 1.创建链表(创建一个表头来表示整个链表)
struct Node *createList()
{
    struct Node *headNode = (struct Node *)malloc(sizeof(struct Node));
    // headNode->data = 1;
    headNode->next = NULL;
    return headNode;
}

// 2.创建节点
//int data: 创建的节点的数据
struct Node *createNode(int data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// 3.遍历/打印 以headNode为头的链表
void printList(struct Node *headNode)
{
    struct Node *pMove = headNode->next; //定义一个移动的指针
    while (pMove)
    {
        printf("%d\n", pMove->data);
        pMove = pMove->next;
    }
}

// 3.插入节点；
// struct Node *headNode : 插入的链表为以headNode为头的链表;
// int data: 插入的数据
void insertNodeByHeadNode(struct Node *headNode, int data)
{
    struct Node *newNode = createNode(data);
    headNode->next = newNode;
    newNode->next = headNode->next;
    newNode->data = data;
}

int main()
{
    //创建链表list
    struct Node *list = createList();
    return 0;
}