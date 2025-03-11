#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Calculator.h"

int main(){
    char expr[MAX_EXPR];
    Situation isError;
    double result;

    while(1){        
        printf("Enter an expression(or 'quit' to exit): ");    

        if(fgets(expr,sizeof(expr),stdin)==NULL){
            printf("Can't read input.\n");
            continue;
        }
        expr[strcspn(expr,"\n")]=0;
        if(strcmp(expr,"quit")==0){
            putchar('\n');
            printf("Goodbye!\n");
            break;
        }

        if((isError=Calculator(expr,&result))!=CALL_OK){
            putchar('\n');
            printf("%s",FindError(isError));
            putchar('\n');
            putchar('\n');
            continue;
        }

        printf("Resule: %g.\n",result);
        putchar('\n');
    }

    return 0;
}