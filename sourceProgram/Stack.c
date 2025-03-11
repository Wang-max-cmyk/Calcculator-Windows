#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Stack.h"

#define DEBUG 0

Stack* StackConstruct(int sizeOfElement){
    Stack *pStack;
    pStack=(Stack*)malloc(sizeof(Stack));
    if(pStack==NULL){
        return NULL;
    }

    pStack->pBase=malloc(STACK_SIZE*sizeOfElement); 
    if(pStack->pBase==NULL){
        free(pStack);
        return NULL; 
    }
    pStack->pTop=pStack->pBase;
    pStack->elementSize=sizeOfElement;
    pStack->stackSize=STACK_SIZE;

    return pStack;
}

Status StackDestruct(Stack* pStack,FreeData freeData){
    if(pStack==NULL){
        return OK;
    }
    StackTraverse(pStack,freeData);
    free(pStack->pBase);
    free(pStack);
    return OK;
}

bool IsStackEmpty(Stack* pStack){
    if(pStack->pBase==pStack->pTop){
        return true;
    }else{
        return false;
    }
}

int StackLength(Stack* pStack){
    return ((pStack->pTop)-(pStack->pBase))/pStack->elementSize;
}

Status StackPush(Stack* pStack,void* pElem){
    if(StackLength(pStack)>=pStack->stackSize){
        (pStack->stackSize)+=STACK_SIZE;
        pStack->pBase=realloc(pStack->pBase,(pStack->stackSize)*(pStack->elementSize));
        if(pStack->pBase==NULL){
            return ERROR;
        }
        pStack->pTop=pStack->pBase+(pStack->stackSize-STACK_SIZE)*(pStack->elementSize);
    }

    if(pStack->pBase==NULL){
        return ERROR;
    }
    
    memcpy(pStack->pTop,pElem,pStack->elementSize);
    pStack->pTop+=pStack->elementSize;
    
    #if DEBUG
        printf("%d ",StackLength(pStack));
    #endif

    return OK; 
}

Status StackPop(Stack* pStack,void* pElem,FreeData freeData){
    if(IsStackEmpty(pStack)){
        return ERROR;
    }else{
        pStack->pTop-=pStack->elementSize;
        memcpy(pElem,pStack->pTop,pStack->elementSize);
        freeData(pStack->pTop);
        return OK;
    }  
}

Status StackGetTop(Stack* pStack,void* pElem){
    if(IsStackEmpty(pStack)){
        return ERROR;
    }
    memcpy(pElem,pStack->pTop-pStack->elementSize,pStack->elementSize);
    return OK;
}

Status StackTraverse(Stack* pStack,Status (*visit)(void*)){
    int i;
    void* pTemp=pStack->pBase;
    for(i=0;i<StackLength(pStack);i++){
        if(visit(pTemp)==ERROR){
            return ERROR;
        }
        pTemp+=pStack->elementSize;
    }
    return OK;
}

Status StackClear(Stack* pStack,FreeData freeData){
    if(StackTraverse(pStack,freeData)==ERROR){
        return ERROR;
    }
    pStack->pTop=pStack->pBase;
    return OK;
}