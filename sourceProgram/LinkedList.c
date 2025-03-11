#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"LinkedList.h"

static ListNode* ListMakeNode(List* pList,void* pData);
/* 功能：构造一个链表节点（私有函数）
参数：pList指向节点所在的链表，pData指向需要存储的信息
返回：构造好的链表节点指针*/

static void ListFreeNode(ListNode* pNode,FreeData freeData);
/* 功能：释放一个链表节点（私有函数）
参数：pNode指向需要free的节点，freeData函数对节点所存数据指向的内存进行free
注意：此处不应该将传入的void*指向的内存free*/

static bool IsThisOne(void* a,void* b,int dataSize);
/* 功能：判断两个指针所存数据是否相同
参数：dataSize是a，b指向内存的字节数
返回：是同一个指针则返回TRUE，否则返回FALSE*/

static ListNode* Merge(ListNode* p1,ListNode* p2,CompareFunction compare);
/* 功能：合并两个已经有序的链表
参数：p1和p2分别是两个字链表的头结点，compare是比较函数
      compare返回1则把b放到a的前面，否则把a放到b的前面
返回：合并后链表的头结点*/

static ListNode* MergeSort(ListNode* head,CompareFunction compare);
/* 功能：利用归并排序对链表进行排序
参数：head是要排序链表的头节点，compare是比较函数
      compare返回1则把b放到a的前面，否则把a放到b的前面
返回：排序好的链表的头结点*/

static ListNode* ListMakeNode(List* pList,void* pData){
    ListNode *pNewNode;

    pNewNode=(ListNode*)malloc(sizeof(ListNode));
    if(pNewNode==NULL){
        return NULL;
    }

    pNewNode->pData=malloc(pList->dataSize);
    if(pNewNode->pData==NULL){
        free(pNewNode);
        return NULL;
    }

    memcpy(pNewNode->pData,pData,pList->dataSize);
    pNewNode->pNextNode=NULL;

    return pNewNode;
}

static void ListFreeNode(ListNode* pNode,FreeData freeData){
    if(pNode==NULL){
        return;
    }
    freeData(pNode->pData);
    free(pNode->pData);
    free(pNode);// 先把数据指向的内存free，再free数据，最后free指向数据的内存
    return;
}

List* ListConstruct(int sizeOfData){
    List* pList=(List*)malloc(sizeof(List));
    if(pList==NULL){
        return NULL;
    }

    pList->pFirstNode=NULL;
    pList->length=0;
    pList->dataSize=sizeOfData;

    return pList;
}

Status ListClear(List* pList,FreeData freeData){
    ListNode *pCurrentNode,*pNextNode;

    pCurrentNode=pList->pFirstNode;
    while(pCurrentNode!=NULL){
        pNextNode=pCurrentNode->pNextNode;
        ListFreeNode(pCurrentNode,freeData);
        pCurrentNode=pNextNode;
    }

    pList->pFirstNode=NULL;
    pList->length=0;

    return OK;
}

Status ListDestruct(List* pList,FreeData freeData){
    ListClear(pList,freeData);
    free(pList);
    return OK;
}

int ListGetLength(List* pList){
    return pList->length;
}

Status ListGetData(ListNode* pNode,void* p,int dataSize){
    memcpy(p,pNode->pData,dataSize);
    return OK;
}

bool ListIsEmpty(List* pList){
    return pList->length==0?true:false;
}

Status ListInsertAtFront(List* pList,void* pData){
    ListNode *pNewNode;
    pNewNode=ListMakeNode(pList,pData);
    if(pNewNode==NULL){
        return ERROR;
    }

    pNewNode->pNextNode=pList->pFirstNode;
    pList->pFirstNode=pNewNode;
    pList->length++;

    return OK;
}

Status ListAppendToEnd(List* pList,void* pData){
    ListNode *pNewNode,*pTail;
    pNewNode=ListMakeNode(pList,pData);
    if(pNewNode==NULL){
        return ERROR;
    }

    if(pList->length==0){
        pList->pFirstNode=pNewNode;
    }else{
        pTail=pList->pFirstNode;
        while(pTail->pNextNode!=NULL){
            pTail=pTail->pNextNode;
        }
        pTail->pNextNode=pNewNode;
    }
    
    pList->length++;

    return OK;
}

Status ListInsertAfter(List* pList,ListNode* pNode,void* pData){
    ListNode* pNewNode;
    pNewNode=ListMakeNode(pList,pData);
    if(pNewNode==NULL){
        return ERROR;
    }

    pNewNode->pNextNode=pNode->pNextNode;
    pNode->pNextNode=pNewNode;
    pList->length++;

    return OK;
}

Status ListDeleteFirstNode(List* pList,FreeData freeData){
    if(ListIsEmpty(pList)){
        return ERROR;
    }

    ListNode *pTempNode;
    pTempNode=pList->pFirstNode;
    pList->pFirstNode=pTempNode->pNextNode;

    ListFreeNode(pTempNode,freeData);
    pTempNode=NULL;
    pList->length--;

    return OK;
}

Status ListDeleteLastNode(List* pList,FreeData freeData){
    ListNode *pTail,*pPreviousNode;
    
    if(ListIsEmpty(pList)){
        return ERROR;
    }

    if(pList->length==1){
        ListFreeNode(pList->pFirstNode,freeData);
        pList->length--;
        pList->pFirstNode=NULL;
        return OK;
    }

    pPreviousNode=pList->pFirstNode;
    pTail=pList->pFirstNode->pNextNode;
    while(pTail->pNextNode!=NULL){
        pPreviousNode=pTail;
        pTail=pTail->pNextNode;
    }
    ListFreeNode(pTail,freeData);
    pTail=NULL;

    pPreviousNode->pNextNode=NULL;

    pList->length--;

    return OK;
}

Status ListDeletNode(List* pList,ListNode* pNode,FreeData freeData){
    ListNode* pTempNode;
    if(pNode==NULL){
        return OK;
    }
    if(pList->length==0){
        return ERROR;
    }

    pTempNode=pList->pFirstNode;
    if(pTempNode==pNode){
        pList->pFirstNode=pNode->pNextNode;
    }else{
        while(pTempNode!=NULL && pTempNode->pNextNode!=pNode){
            pTempNode=pTempNode->pNextNode;
        }

        if(pTempNode==NULL){
            return ERROR;
        }

        pTempNode->pNextNode=pNode->pNextNode;
    }

    ListFreeNode(pNode,freeData);
    pNode=NULL;
    pList->length--;

    return OK;
}

bool IsThisOne(void* a,void* b,int dataSize){
    char *pa,*pb;
    pa=(char*)a;
    pb=(char*)b;

    for(int i=0;i<dataSize;i++){
        if(pa[i]!=pb[i]){
            return false;
        }
    }

    return true;
}

ListNode* ListSearch(List* pList,void* pKey){
    ListNode* pTempNode;
    pTempNode=pList->pFirstNode;

    while(pTempNode!=NULL){
        if(IsThisOne(pTempNode->pData,pKey,pList->dataSize)==true){
            return pTempNode;
        }
        pTempNode=pTempNode->pNextNode;
    }

    return NULL;
}

Status ListTraverse(List* pList,Status (*visit)(void*)){
    ListNode* pTempNode=pList->pFirstNode;

    while(pTempNode!=NULL){
        if(visit(pTempNode->pData)==ERROR){
            return ERROR;
        }
        pTempNode=pTempNode->pNextNode;
    }

    return OK;
}

ListNode* Merge(ListNode* p1,ListNode* p2,CompareFunction compare){
    if(p1==NULL){
        return p2;
    }
    if(p2==NULL){
        return p1;
    }

    ListNode* dummy=(ListNode*)malloc(sizeof(ListNode));
    ListNode* curr=dummy;

    while(p1 && p2){
        if(compare(p1,p2)!=1){
            curr->pNextNode=p1;
            p1=p1->pNextNode;
        }else{
            curr->pNextNode=p2;
            p2=p2->pNextNode;
        }
        curr=curr->pNextNode;
    }

    if(p1){
        curr->pNextNode=p1;
    }
    if(p2){
        curr->pNextNode=p2;
    }

    ListNode* result=dummy->pNextNode;
    free(dummy);
    return result;
}

ListNode* MergeSort(ListNode* head,CompareFunction compare){
    if((!head) || (!head->pNextNode)){
        return head;
    }

    ListNode* slow=head;
    ListNode* fast=head->pNextNode;
    while(fast && fast->pNextNode){
        slow=slow->pNextNode;
        fast=fast->pNextNode->pNextNode;
    }

    ListNode* mid=slow->pNextNode;
    slow->pNextNode=NULL;
    ListNode* left=MergeSort(head,compare);
    ListNode* right=MergeSort(mid,compare);

    return Merge(left,right,compare);
}

Status Sort(List* pList,CompareFunction compare){
    if((!pList) || (!pList->pFirstNode)){
        return ERROR;
    }

    pList->pFirstNode=MergeSort(pList->pFirstNode,compare);

    return OK;
}