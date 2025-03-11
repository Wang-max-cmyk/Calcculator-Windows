#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"Calculator.h"

#define DEBUG 0

static bool IsDigit(char c);
/* 功能：判断传入的字符是不是数字0到9
返回：是则返回TRUE，否则返回FALSE*/

static bool IsAlpha(char c);
/* 功能：判断传入的字符是不是字母
返回：是则返回TRUE，否则返回FALSE*/

static Situation Process(char* expr);
/* 功能：对传入的字符串初步加工(主要是处理正负号)，以便能方便地传化为词法单元并判断括号嵌套是否正确，是否输入
返回：过程顺利进行则返回CALL_OK，否则返回遇到的错误信息*/

static Situation ConvertToDouble(char* s,double* result);
/* 功能：将一个字符数组转为一个双精度浮点型并判断小数点是否正确
参数：s为需要转为双精度浮点型的字符数组，result为转化为的数放入的地址
返回：过程顺利进行则返回CALL_OK，否则返回遇到的错误信息*/

static int Prority(char c);
/* 功能：判断传入运算符的优先级*/

static Situation PerformOperation(double operand1,double operand2,char operator,double* result);
/* 功能：将两个double类型的数进行运算
参数：operand1，operand2表示数，operator表示运算符（注意顺序）,result为计算结果放入的地址
返回：过程顺利进行则返回CALL_OK，否则返回遇到的错误信息*/

static Situation GetToken(int *pExpr,char* expr,Token *pToken);
/* 功能：将一个字符转化为词法单元
参数：pExpr表示需要转化的字符在字符数组expr中的位置，expr是包含字符的数组，pToken是转化为的词法单元放入的地方
返回：过程顺利进行则返回CALL_OK，否则返回遇到的错误信息*/

static Situation Calculate(Token* p,double* result);
/* 功能：将一个词法单元数组进行计算
参数：p为需要计算的词法单元数组，result为计算结果放入的地址
返回：过程顺利进行则返回CALL_OK，否则返回遇到的错误信息*/

static Situation CheckError(Token* p);
/* 功能：检查是否缺少操作符以及操作符后是否缺少操作数
参数：需要检查的Token数组
返回：过程顺利进行则返回CALL_OK，否则返回遇到的错误信息*/

static Status PrintTokens(Token* p);
/* 功能：将Token数组p依次打印出来*/

static Status FreeChar(void* p);
/* 功能：free p指针指向内存所指向的内存（即**p）（由于这里p指向的不是指针，实际并未进行任何操作）
返回：过程顺利进行则返回OK，否则返回ERROR*/

static Status FreeDouble(void* p);
/* 功能：free p指针指向内存所指向的内存（即**p）（由于这里p指向的不是指针，实际并未进行任何操作）
返回：过程顺利进行则返回OK，否则返回ERROR*/

bool IsDigit(char c){
    if(c>='0' && c<='9'){
        return true;
    }else{
        return false;
    }
}

bool IsAlpha(char c){
    if((c>=65 && c<=90) || (c>=97 && c<=122)){
        return true;
    }else{
        return false;
    }
}

Situation Process(char* expr){
    if((*expr)=='\0'){
        return ERROR15;
    }

    int i,length,lNum=0,rNum=0;
    for(length=0;expr[length]!='\0';length++){
        if(expr[length]=='(' && expr[length+1]==')'){
            return ERROR1;
        }

        if(expr[length]=='('){
            lNum++;
        }
        if(expr[length]==')'){
            rNum++;
        }

        if(lNum<rNum){
            return ERROR2;
        }
    }
    if(lNum!=rNum){
        return ERROR3;
    }

    if(expr[0]=='+' || *expr=='-'){
        i=length;
        expr[i+1]='\0';
        for(;i>0;i--){
            expr[i]=expr[i-1];
        }
        length++;
        expr[0]='0';
    }

    for(i=0;expr[i+2]!='\0';i++){
        if(expr[i]=='(' && (expr[i+1]=='+' || expr[i+1]=='-')){
            expr[length+1]='\0';
            int j=length;
            for(;j>(i+1);j--){
                expr[j]=expr[j-1];
            }
            expr[i+1]='0';
            length++;
        }
    }

    return CALL_OK;
}

Situation ConvertToDouble(char* s,double* result){
    int i,pointNum=0;
    double value=0;
    for(i=0;s[i]!='\0';i++){
        if(s[i]=='.'){
            pointNum++;
        }
    }
    if(pointNum>=2){
        return ERROR4;
    }

    i=0;
    while(s[i] && s[i]!='.'){
        value=value*10+(double)(s[i]-'0');
        i++;
    }
    if(!s[i]){
        (*result)=value;
        return CALL_OK;
    }
    i++;
    double weight=0.1;
    while(s[i]){
        value=value+(double)((s[i]-'0')*weight);
        weight/=10;
        i++;
    }
    (*result)=value;
    return CALL_OK;
}

int Prority(char c){
    switch(c){
        case '(':return 1;
        case '+':
        case '-':return 2;
        case '*':
        case '/':return 3;
        case '^':return 4;
        default:return 0;
    }
}

Situation PerformOperation(double operand1,double operand2,char operator,double* result){
    switch(operator){
        case '+':(*result)=operand1+operand2; break;
        case '-':(*result)=operand1-operand2; break;
        case '*':(*result)=operand1*operand2; break;
        case '/':
            if(operand2==0){
                return ERROR5;
            }
            (*result)=operand1/operand2; break;
        case '^':
            if(operand1==0 && operand2<=0){
                return ERROR6;
            }
            (*result)=pow(operand1,operand2);
    }
    return CALL_OK;
}

Situation GetToken(int *pExpr,char* expr,Token *pToken){
    Token current_token;
    while(expr[*pExpr]==' '){
        (*pExpr)++;
    }

    if(expr[*pExpr]=='\0'){
        current_token.type=END;
        current_token.value[0]='\0';
        (*pToken)=current_token;
        return CALL_OK;
    }

    if(IsDigit(expr[*pExpr]) || expr[*pExpr]=='.'){
        current_token.type=NUMBER;
        int num_len=0;
        while(IsDigit(expr[*pExpr]) || expr[*pExpr]=='.'){
            current_token.value[num_len++]=expr[*pExpr];
            (*pExpr)++;
        }
        current_token.value[num_len]='\0';
        (*pToken)=current_token;
        return CALL_OK;
    }

    if(IsAlpha(expr[*pExpr])){
        int i=0;

        #if DEBUG
            printf("Sucess1.\n");
        #endif
        
        while(IsAlpha(expr[*pExpr])){
            if(expr[*pExpr]){
                current_token.value[i++]=expr[(*pExpr)++];
            }
        }

        #if DEBUG
            printf("Sucess2.\n");
        #endif

        current_token.value[i]='\0';

        if(strcmp(current_token.value,"PI")==0){
            current_token.type=CONSTANT;
            (*pToken)=current_token;
            return CALL_OK;
        }
        if(strcmp(current_token.value,"e")==0){
            current_token.type=CONSTANT;
            (*pToken)=current_token;
            return CALL_OK;
        }

        current_token.type=FUNCTION;
        (*pToken)=current_token;
        return CALL_OK;
    }

    switch(expr[*pExpr]){
        case '(':
            current_token.type=LPAREN;
            current_token.value[0]='(';
            current_token.value[1]='\0';
            break;
        case '+':
            current_token.type=OPERATOR;
            current_token.value[0]='+';
            current_token.value[1]='\0';
            break;
        case '-':
            current_token.type=OPERATOR; 
            current_token.value[0]='-';
            current_token.value[1]='\0';
            break;
        case '*':
            current_token.type=OPERATOR; 
            current_token.value[0]='*';
            current_token.value[1]='\0';
            break;
        case '/':
            current_token.type=OPERATOR; 
            current_token.value[0]='/';
            current_token.value[1]='\0';
            break;
        case '^':
            current_token.type=OPERATOR;
            current_token.value[0]='^';
            current_token.value[1]='\0';
            break;
        case ')':
            current_token.type=RPAREN;
            current_token.value[0]=')';
            current_token.value[1]='\0';
            break;
        default: return ERROR7;
    }
    (*pExpr)++;

    *(pToken)=current_token;
    return CALL_OK;
}

Situation Calculate(Token* p,double* result){
    Stack *operand,*operator;
    operand=StackConstruct(sizeof(double));
    operator=StackConstruct(sizeof(char));
    StackPush(operator,"#");
    int i,loc=0;
    char top;
    Situation isError;

    while(1){
        if(p[loc].type==END){
            break;
        }
    #if DEBUG
        printf("again\n");
    #endif
        if(p[loc].type==CONSTANT){
            double value;
            if(strcmp(p[loc].value,"PI")==0){
                value=PI;
                StackPush(operand,&value);
            }
            if(strcmp(p[loc].value,"e")==0){
                value=e;
                StackPush(operand,&value);
            }
            loc++;
            continue;
        }
        if(p[loc].type==NUMBER){
            double mid_result;
            if((isError=ConvertToDouble(p[loc++].value,&mid_result))!=CALL_OK){
                return isError;
            }
            
            #if DEBUG
                printf("%s %lf\n",p[loc-1].value,mid_result);
            #endif

            StackPush(operand,&mid_result);
            continue;
        }else{
            if(p[loc].type==FUNCTION){
                char func[MAX_VALUE_LENGTH];
                strcpy(func,p[loc++].value);

                if(p[loc].type!=LPAREN){
                    return ERROR8;
                }
                i=1;
                Token cal[MAX_EXPR];
                int cal_len=0;
                loc++;
                while(1){
                    if(p[loc].type==LPAREN){
                        i++;
                    }
                    if(p[loc].type==RPAREN){
                        i--;
                    }
                    if(i==0){
                        cal[cal_len++].type=END;
                        break;
                    }
                    cal[cal_len++]=p[loc++];
                }
                double ps,funcPs;
                if((isError=Calculate(cal,&ps)!=CALL_OK)){
                    return isError;
                }
                if(strcmp(func,"sin")==0){
                    funcPs=sin(ps);
                }else if(strcmp(func,"asin")==0){
                    funcPs=asin(ps);
                }else if(strcmp(func,"cos")==0){
                    funcPs=cos(ps);
                }else if(strcmp(func,"acos")==0){
                    funcPs=acos(ps);
                }else if(strcmp(func,"tan")==0){
                    funcPs=tan(ps);
                }else if(strcmp(func,"atan")==0){
                    funcPs=atan(ps);
                }else if(strcmp(func,"lg")==0){
                    if(ps<=0){
                        return ERROR9;
                    }
                    funcPs=log10(ps);
                }else if(strcmp(func,"ln")==0){
                    if(ps<=0){
                        return ERROR10;
                    }
                    funcPs=log(ps);
                }else if(strcmp(func,"sqrt")==0){
                    if(ps<0){
                        return ERROR11;
                    }
                    funcPs=sqrt(ps);
                }else if(strcmp(func,"fabs")==0){
                    funcPs=fabs(ps);
                }else{
                    return ERROR12;
                }
                
                StackPush(operand,&funcPs);
                loc++;
                continue;
            }

            if(p[loc].type==LPAREN){
                i=1;
                Token cal[MAX_EXPR];
                int cal_len=0;
                loc++;
                while(1){
                    if(p[loc].type==LPAREN){
                        i++;
                    }
                    if(p[loc].type==RPAREN){
                        i--;
                    }
                    if(i==0){
                        cal[cal_len++].type=END;
                        break;
                    }
                    cal[cal_len++]=p[loc++];
                }
                double calL;
                if((isError=Calculate(cal,&calL)!=CALL_OK)){
                    return isError;
                }

                #if DEBUG
                    printf("Success.\n");
                #endif

                StackPush(operand,&calL);

                #if DEBUG
                    printf("Success.\n");
                #endif        

                loc++;
                continue;
            }

            StackGetTop(operator,&top);
            
            if(Prority(p[loc].value[0])>Prority(top)){
                StackPush(operator,&(p[loc++].value[0]));
                continue;
            }else if(Prority(p[loc].value[0])<=Prority(top)){
                while(Prority(p[loc].value[0])<=Prority(top)){
                    double operand1,operand2,mid_result;
                    char ope;
                    StackPop(operand,&operand2,FreeDouble);
                    StackPop(operand,&operand1,FreeDouble);
                    StackPop(operator,&ope,FreeChar);
                    if((isError=PerformOperation(operand1,operand2,ope,&mid_result))!=CALL_OK){
                        return isError;
                    }
                    StackPush(operand,&mid_result);

                    #if DEBUG
                        printf("%lf %s %lf=%lf\n",operand1,ope,operand1,mid_result);
                    #endif
                    
                    StackGetTop(operator,&top);
                }
                StackPush(operator,&(p[loc++].value[0]));
                continue;
            }
        }
    }

    #if DEBUG
        printf("success.");
    #endif

    StackGetTop(operator,&top);
    while(top!='#'){
        double operand1,operand2,mid_result;
        char ope;
        StackPop(operand,&operand2,FreeDouble);
        StackPop(operand,&operand1,FreeDouble);
        StackPop(operator,&ope,FreeChar);
        if((isError=PerformOperation(operand1,operand2,ope,&mid_result))!=CALL_OK){
            return isError;
        }
        StackPush(operand,&mid_result);

        #if DEBUG
            printf("%d = %d %c %d",mid_result,operand1,ope,operand2);
        #endif

        StackGetTop(operator,&top);
    }

    #if DEBUG
        printf("success.");
    #endif

    StackPop(operand,result,FreeDouble);
    if((*result)>=-ACCURACY && (*result)<=ACCURACY){
        (*result)=0;
    }
    StackDestruct(operand,FreeDouble);
    StackDestruct(operator,FreeChar);
    return CALL_OK;
}

Situation CheckError(Token* p){
    int i=0;
    while(p[i].type!=END){
        if(p[i].type==OPERATOR){
            if(p[i+1].type!=NUMBER && p[i+1].type!=FUNCTION && p[i+1].type!=LPAREN && p[i+1].type!=CONSTANT){
                return ERROR13;
            }
        }

        if((p[i].type==NUMBER || p[i].type==CONSTANT) && p[i+1].type!=END && p[i+1].type!=OPERATOR && p[i+1].type!=RPAREN){
            return ERROR14;
        }

        i++;
    }

    return CALL_OK;
}

Status PrintTokens(Token* p){
    putchar('\n');
    int i=0;
    while(p[i].type!=END){
        printf("p[%d].value is %s and type is %d.\n",i,p[i].value,p[i].type);
        i++;
    }
    putchar('\n');
    return OK;
}

Status FreeChar(void* p){
    char* pc=(char*)p;
    return OK;
}

Status FreeDouble(void* p){
    double* pd=(double*)p;
    return OK;
}

Situation Calculator(char* expr,double* result){
    Situation isError;
    if((isError=Process(expr))!=CALL_OK){
        return isError;
    }

    Token p[MAX_EXPR];
    int loc=0,len=0;

    while(1){
        if((isError=GetToken(&loc,expr,&p[len++]))!=CALL_OK){
            return isError;
        }
        if(p[len-1].type==END){
            break;
        }
    }

    #if DEBUG
        PrintTokens(p);
    #endif

    if((isError=CheckError(p))!=CALL_OK){
        return isError;
    }

    if((isError=Calculate(p,result))!=CALL_OK){
        return isError;
    }
    
    #if DEBUG
        PrintTokens(p);
    #endif

    return CALL_OK;
}

char* FindError(Situation error){
    if(error==CALL_OK){
        return "There is no error.";
    }else if(error==ERROR1){
        return "The expression includes empty parentheses.";
    }else if(error==ERROR2){
        return "The parentheses are nested incorrectly.";
    }else if(error==ERROR3){
        return "The number of '(' isn't equal to the number of ')'.";
    }else if(error==ERROR4){
        return "The inputted number is illegal.";
    }else if(error==ERROR5){
        return "Division by zero.";
    }else if(error==ERROR6){
        return "The exponent of 0 must be greater than 0.";
    }else if(error==ERROR7){
        return "Invalid character.";
    }else if(error==ERROR8){
        return "It's should be '(' after function.";
    }else if(error==ERROR9){
        return "Invalid argument for lg.";
    }else if(error==ERROR10){
        return "Invalid argument for ln.";
    }else if(error==ERROR11){
        return "Invalid argument for sqrt.";
    }else if(error==ERROR12){
        return "The function doesn't exist.";
    }else if(error==ERROR13){
        return "It should be a number after operator.";
    }else if(error==ERROR14){
        return "It may be a fault after operator.";
    }else if(error==ERROR15){
        return "You don't input.";
    }else{
        return "Unknown error";
    }
}