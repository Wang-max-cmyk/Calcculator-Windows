#ifndef CALCULATOR_H
#define CALCULATOR_H

#include<stdbool.h>

#include"Status.h"
#include"Stack.h"

#define MAX_EXPR 100
#define MAX_VALUE_LENGTH 20
#define PI 3.141592653589
#define e 2.71828
#define ACCURACY 0.00001
// 定义了表达式的最大长度。一个词法单元最多字符数。常数PI和e.精度（用于当结果与该值的距离小于0时直接赋值为0）。

typedef enum{UNKNOWN_ERROR=0,CALL_OK=1,ERROR1,ERROR2,ERROR3,ERROR4,ERROR5,ERROR6,
ERROR7,ERROR8,ERROR9,ERROR10,ERROR11,ERROR12,ERROR13,ERROR14,ERROR15}Situation;
// 定义一个枚举类型Situation表示某个过程成功或失败
/*UNKNOWN_ERROR:未知错误
CALL_OK:成功
ERROR1:The expression includes empty parentheses.
ERROR2:The parentheses are nested incorrectly.
ERROR3:The number of '(' isn't equal to the number of ')'.
ERROR4:The inputted number is illegal.
ERROR5:Division by zero.
ERROR6:The exponent of 0 must be greater than 0.
ERROR7:Invalid character.
ERROR8:It's should be '(' after function.
ERROR9:Invalid argument for lg.
ERROR10:Invalid argument for ln.
ERROR11:Invalid argument for sqrt.
ERROR12:The function doesn't exist.
ERROR13:It should be a number after operator.
ERROR14:It may be a fault after operator.
ERROR15:You don't input.*/

typedef enum{
    NUMBER,OPERATOR,LPAREN,RPAREN,FUNCTION,CONSTANT,END
}TokenType;
// 定义了词法单元的类型

typedef struct{
    TokenType type;
    char value[MAX_EXPR];
}Token;
// 定义了词法单元结构

Situation Calculator(char* expr,double* result);
/* 功能：将一个字符数组进行计算
参数：需要计算的字符数组expr，result为计算结果放入的地址
返回：过程顺利进行则返回CALL_OK，否则返回遇到的错误信息*/

char* FindError(Situation error);
/* 功能：根据计算过程中的返回值发现遇到的错误
参数：Situation的返回值
返回：蕴含错误信息的字符串*/

#endif