#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"LinkedStack.h"

Stack* StackConstruct(int sizeOfElement){
    return ListConstruct(sizeOfElement);
}

Status StackDestruct(Stack* pStack,FreeData freeData){
    return ListDestruct(pStack,freeData);
}

bool IsStackEmpty(Stack* pStack){
    return ListIsEmpty(pStack);
}

int StackLength(Stack* pStack){
    return ListGetLength(pStack);
}

Status StackPush(Stack* pStack,void* pElem){
    return ListInsertAtFront(pStack,pElem);
}

Status StackPop(Stack* pStack,void* pElem,FreeData freeData){
    if(ListGetData(pStack->pFirstNode,pElem,pStack->dataSize)==ERROR){
        return ERROR;
    }
    if(ListDeleteFirstNode(pStack,freeData)==ERROR){
        return ERROR;
    }
    return OK;
}

Status StackGetTop(Stack* pStack,void* pElem){
    return ListGetData(pStack->pFirstNode,pElem,pStack->dataSize);
}

Status StackTraverse(Stack* pStack,Status (*visit)(void*)){
    return ListTraverse(pStack,visit);
}

Status StackClear(Stack* pStack,FreeData freeData){
    return ListClear(pStack,freeData);
}