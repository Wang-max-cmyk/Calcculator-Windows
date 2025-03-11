#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include"LinkedList.h"

#define LIST_LENTH 20
#define MAX_VALUE 100

int Compare(ListNode* a,ListNode* b);
Status FreeInt(void* p);
Status PrintInt(void* p);

int Compare(ListNode* a,ListNode* b){
    int *pa=(int*)(a->pData);
    int *pb=(int*)(b->pData);

    if((*pa)>(*pb)){
        return 1;
    }else if((*pa)==(*pb)){
        return 0;
    }else{
        return -1;
    }
}

Status FreeInt(void* p){
    int *pi=(int*)p;
    return OK;
}

Status PrintInt(void* p){
    int *pi=(int*)p;
    printf("%d\t",*pi);
    return OK;
}

int main(){
    int i;
    ListNode* pNode;
    List* p;
    p=ListConstruct(sizeof(int));
    if(!p){
        printf("ERROR:List construction failed.\n");
        return 1;
    }

    int temp;
    srand((unsigned)time(NULL));
    for(i=0;i<LIST_LENTH-1;i++){
        temp=rand()%MAX_VALUE;
        if(ListInsertAtFront(p,&temp)==ERROR){
            printf("ERROR:List insertion failed.\n");
            return 1;
        }
    }
    printf("Insert at front:\n");
    ListTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    int key=100;
    ListInsertAfter(p,p->pFirstNode->pNextNode,&key);
    printf("Insert 100 in third: ");
    ListTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    key=100;
    pNode=ListSearch(p,&key);
    printf("Search the node whose value is 100:\n");
    PrintInt(pNode->pData);
    putchar('\n');
    putchar('\n');

    Sort(p,Compare);
    printf("Sort:\n");
    ListTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    if(ListDeletNode(p,pNode,FreeInt)==ERROR){
        printf("List deletion failed.\n");
    }
    printf("Delet 100:\n");
    ListTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    ListClear(p,FreeInt);
    if(p->pFirstNode==NULL){
        printf("Clear successully.\n");
    }
    if(ListIsEmpty(p)){
        printf("List is empty.\n");
    }
    putchar('\n');
    putchar('\n');

    for(i=0;i<LIST_LENTH;i++){
        key=rand()%MAX_VALUE;
        if(ListAppendToEnd(p,&key)==ERROR){
            printf("ERROR:List insertion failed.\n");
            return 1;
        }
    }
    printf("Insert append to end:\n");
    ListTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    ListDeleteFirstNode(p,FreeInt);
    printf("Delete first node:\n");
    ListTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    ListDeleteLastNode(p,FreeInt);
    printf("Delete last node:\n");
    ListTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    int length=ListGetLength(p);
    printf("p's length: %d",length);
    putchar('\n');
    putchar('\n');

    int* pi;
    pi=(int*)malloc(sizeof(int));
    ListGetData(p->pFirstNode,pi,p->dataSize);
    printf("Get first data: %d",*pi);
    free(pi);

    ListDestruct(p,FreeInt);
    p=NULL;
    return 0;
}