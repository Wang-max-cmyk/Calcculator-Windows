#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include<stdbool.h>

#include"Status.h"
// 包含Status的定义

typedef struct ListNode{
    void* pData;
    struct ListNode* pNextNode;
}ListNode;
// 定义链表节点

typedef struct{
    ListNode* pFirstNode;
    int length;
    int dataSize;
}List;
// 定义链表

typedef Status (*FreeData)(void*);
/* 功能：定义一个函数指针来free(*void)（如果*void是指针的话）
返回：过程顺利则返回OK，否则返回ERROR
注意：FreeData函数free栈中存储数据指向的内存及而不要free pBase指向的内存（即数据所在的内存）*/

typedef int (*CompareFunction)(ListNode* a,ListNode* b);
/* 功能：定义一个函数指针对节点a，b进行比较
参数：需要比较的两个节点
返回：返回1则把b放到a的前面，否则把a放到b的前面*/

List* ListConstruct(int sizeOfData);
/* 功能：构造一个链表
参数：所存数据的大小
返回：构造好的链表指针*/

Status ListClear(List* pList,FreeData freeData);
/* 功能：清空一个链表所存的信息
参数：pList指向需要清空的链表，freeData函数对节点所存数据指向的内存进行free
注意：此处不应该将传入的void*指向的内存free
返回：过程顺利则返回OK，否则返回ERROR*/

Status ListDestruct(List* pList,FreeData freeData);
/* 功能：析构一个链表
参数：pList指向需要析构的链表，freeData函数对节点所存数据指向的内存进行free
注意：此处不应该将传入的void*指向的内存free
返回：过程顺利则返回OK，否则返回ERROR*/

int ListGetLength(List* pList);
/* 功能：计算链表长度（节点个数）
参数：需要计算长度的链表
返回：链表长度*/

Status ListGetData(ListNode* pNode,void* p,int dataSize);
/* 功能：从一个链表节点中取出数据
参数：需要取出数据的节点pNode，需要放入数据的p，数据的大小dataSize
返回：过程顺利则返回OK，否则返回ERROR*/

bool ListIsEmpty(List* pList);
/* 功能：判断一个链表是否为空
返回：空则返回TRUE，否则返回FALSE*/

Status ListInsertAtFront(List* pList,void* pData);
/* 功能：向链表头部插入数据
参数：pList指向需要插入数据的链表，pData为需要插入数据的指针
返回：过程顺利则返回OK，否则返回ERROR*/

Status ListAppendToEnd(List* pList,void* pData);
/* 功能：向链表尾部插入数据
参数：pList指向需要插入数据的链表，pData为需要插入数据的指针
返回：过程顺利则返回OK，否则返回ERROR*/

Status ListInsertAfter(List* pList,ListNode* pNode,void* pData);
/* 功能：在链表某节点后插入数据
参数：pList指向需要插入数据的链表，pNode表示需要在该节点之后插入数据，pData为需要插入数据的指针
返回：过程顺利则返回OK，否则返回ERROR*/

Status ListDeleteFirstNode(List* pList,FreeData freeData);
/* 功能：删除链表头节点
参数：pList指向需要删除头节点的链表，freeData函数对节点所存数据指向的内存进行free
返回：过程顺利则返回OK，否则返回ERROR
注意：此处不应该将传入的void*指向的内存free*/

Status ListDeleteLastNode(List* pList,FreeData freeData);
/* 功能：删除链表尾节点
参数：pList指向需要删除尾节点的链表，freeData函数对节点所存数据指向的内存进行free
返回：过程顺利则返回OK，否则返回ERROR
注意：此处不应该将传入的void*指向的内存free*/

Status ListDeletNode(List* pList,ListNode* pNode,FreeData freeData);
/* 功能：删除链表指定节点
参数：pList指向需要删除节点的链表，pNode为需要删除的节点，freeData函数对节点所存数据指向的内存进行free
返回：过程顺利则返回OK，否则返回ERROR
注意：此处不应该将传入的void*指向的内存free*/

ListNode* ListSearch(List* pList,void* pKey);
/* 功能：找到链表中的指定节点(找到的是第一个符合条件的节点)
参数：pList为需要search的链表指针，pKey的作用：要找到节的点所存的数据为pKey指向的数据（并不要求指针一样，只有数据一样即可）
返回：找到的节点指针*/

Status ListTraverse(List* pList,Status (*visit)(void*));
/* 功能：遍历链表，对链表中的数据进行visit操作
参数：pList指向需要遍历的链表，visit为对数据进行操作的函数
返回：过程顺利则返回OK，否则返回ERROR*/

Status Sort(List* pList,CompareFunction compare);
/* 功能：对链表进行排序
参数：pList是要排序链表的头结点，compare是比较函数
      compare返回1则把b放到a的前面，否则把a放到b的前面
返回：过程顺利则返回OK，否则返回ERROR*/

#endif