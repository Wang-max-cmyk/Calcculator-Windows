#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#include<stdbool.h>

#include"Status.h"
// 包含Status的定义
#include"LinkedList.h"

typedef List Stack;
// 用链表定义一个栈

typedef Status (*FreeData)(void*);
/* 功能：定义一个函数指针来free(*void)（如果*void是指针的话）
返回：过程顺利则返回OK，否则返回ERROR
注意：FreeData函数free栈中存储数据指向的内存及而不要free pBase指向的内存（即数据所在的内存）*/

Stack* StackConstruct(int sizeOfElement);
/* 功能：构造一个栈。
参数：sizeOfElement是栈元素类型的长度。
返回：如果构造成功，函数返回指向栈的指针；如果失败，返回NULL。*/

Status StackDestruct(Stack* pStack,FreeData freeData);
/* 功能：Destruct函数是Construct函数的逆操作，它销毁pStack指向的栈。
注意：freeData函数free栈中存储数据指向的内存及而不要free pBase指向的内存（即数据）.
返回：如果成功，返回OK；如果失败，返回ERROR*/

bool IsStackEmpty(Stack* pStack);
/* 功能：判断传入的栈是否为空。
返回：如果栈是空的，返回TRUE，否则返回FALSE。*/

int StackLength(Stack* pStack);
/* 功能：计算传入栈的长度。
返回：传入栈的长度。*/

Status StackPush(Stack* pStack,void* pElem); 
/* 功能：将数据元素压入栈。
参数：pStack是栈指针；pElem指向要进栈的数据元素。
返回：如果成功，返回OK；如果失败，返回ERROR。*/

Status StackPop(Stack* pStack,void* pElem,FreeData freeData); 
/* 功能：弹出栈顶元素。
参数：pStack是栈指针，弹出的元素存入pElem指向的内存中。
返回：如果成功，返回OK；如果失败，返回ERROR。*/

Status StackGetTop(Stack* pStack,void* pElem);
/* 功能：将栈顶数据元素取出，实际并未取出，只是看一下。
参数：pStack是栈指针；pElem指向要赋值栈顶数据元素的元素（如果栈为空，pElem将不被操作）。
返回：如果成功，返回OK；如果栈为空，返回ERROR。*/

Status StackTraverse(Stack* pStack,Status (*visit)(void*));
/* 功能：遍历一个栈，对栈中每一个元素进行visit操作。
参数：pStack指向要遍历的栈，visit是对栈中元素的操作。
返回：如果成功，返回OK；如果失败，返回ERROR。*/

Status StackClear(Stack* pStack,FreeData freeData);
/*功能：清空一个栈。
参数：pStack指向要清空的栈。
返回：如果成功，返回OK；如果失败，返回ERROR。*/

#endif