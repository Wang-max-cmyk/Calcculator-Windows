#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include"Calculator.h"

int main(){
    FILE *inputFile,*outputFile,*isEmpty;
    isEmpty=fopen("output.txt","r");
    if(isEmpty==NULL){
        perror("I can't read output.txt.\n");
        exit(1);
    }
    inputFile=fopen("input.txt","rb");
    if(inputFile==NULL){
        perror("I can't open input.txt.\n");
        exit(1);
    }
    outputFile=fopen("output.txt","ab");
    if(outputFile==NULL){
        perror("I can't open output.txt.\n");
        exit(1);
    }

    char expr[MAX_EXPR];
    Situation isError;

    if(fgets(expr,MAX_EXPR*sizeof(char),inputFile)==NULL){
        printf("You don't input.\n");
        exit(1);
    }

    if(!(fgetc(isEmpty)==EOF)){
        putc('\n',outputFile);
        putc('\n',outputFile);
    }
    fclose(isEmpty);

    time_t current_time=time(NULL);
    struct tm *local_time;
    local_time=localtime(&current_time);
    fprintf(outputFile,"Present time: %d.%02d.%02d %02d:%02d:%02d",local_time->tm_year+1900,
    local_time->tm_mon+1,local_time->tm_mday,local_time->tm_hour,local_time->tm_min,local_time->tm_sec);
    putc('\n',outputFile);

    int i,c;
    double result;
    while(1){
        for(i=0;expr[i]!='\0';i++){
            if(expr[i]=='\r'){
                expr[i]='\0';
                break;
            }
        }
        
        i=0;
        while((c=expr[i])!='\0'){
            putc(c,outputFile);
            i++;
        }

        if((isError=Calculator(expr,&result))!=CALL_OK){
            fprintf(outputFile,"\tERROR: %s",FindError(isError));
        }else{
            fprintf(outputFile,"=%g",result);
        }
        
        if(fgets(expr,MAX_EXPR*sizeof(char),inputFile)==NULL){
            break;
        }

        putc('\n',outputFile);
    }
    
    fclose(inputFile);
    fclose(outputFile);

    fprintf(stdout,"Calculate successfully.\n");

    return 0;
}