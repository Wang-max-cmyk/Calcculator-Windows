#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include"Stack.h"

#define MAX_VALUE 100
#define SIZE 110

Status FreeInt(void* p);
Status PrintInt(void* p);

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
    Stack* p=StackConstruct(sizeof(int));
    if(!p){
        printf("ERROR:List construction failed.\n");
        return 1;
    }
    srand((unsigned)time(NULL));

    int value;
    for(int i=0;i<SIZE;i++){
        value=rand()%MAX_VALUE;
        StackPush(p,&value);
    }
    printf("Assign value:\n");
    StackTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    int* temp=(int*)malloc(sizeof(int));
    StackGetTop(p,temp);
    printf("top: %d\n",*temp);
    putchar('\n');
    putchar('\n');

    StackPop(p,temp,FreeInt);
    printf("Pop: %d\n",*temp);
    StackTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    (*temp)=StackLength(p);
    printf("Length: %d",*temp);
    putchar('\n');
    putchar('\n');

    (*temp)=100;
    StackPush(p,temp);
    printf("Push 100:\n");
    StackTraverse(p,PrintInt);
    putchar('\n');
    putchar('\n');

    StackClear(p,FreeInt);
    if(StackLength(p)==0){
        printf("Clear Successfully.\n");
    }
    if(IsStackEmpty(p)){
        printf("Stack is empty.\n");
    }

    StackDestruct(p,FreeInt);
    p=NULL;
    free(temp);
    return 0;
}