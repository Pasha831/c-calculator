#ifndef C_CALCULATOR_VARIABLES_H
#define C_CALCULATOR_VARIABLES_H
#include <tgmath.h>
#define MAXSIZE 100
#define MAX_VARS 10

typedef struct Var Var;
typedef struct Data Data;

typedef struct Var {
    char name[MAXSIZE];
    double complex value;
    int isInit;

    char polish[MAXSIZE][MAXSIZE];  // RPN
    int countPolish;  // pointer for RPN

    int unknown;  // number of linked variables, that are still undefined
    Var *linked[MAX_VARS];
    int countLinked;
} Var;

typedef struct Data {
    int count;
    Var variables[MAX_VARS];
} Data;

void initData(Data *data);
void initVar(Var *var);
int inData(char *varName, Data *data);
void addVar(char *varName, Data *data);

#endif
