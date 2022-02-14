#ifndef C_CALCULATOR_VARIABLES_H
#define C_CALCULATOR_VARIABLES_H
#include <tgmath.h>
#define MAXSIZE 100
#define MAX_VARS 100
#define COUNT_CONST 2

typedef struct Var Var;
typedef struct Data Data;

typedef struct Var {
    char name[MAXSIZE];
    double complex value;
    int isDefined;

    char polish[MAXSIZE][MAXSIZE];  // RPN
    int countPolish;  // pointer for RPN

    int countUnknown;  // number of linked variables, that are still undefined
    Var *childrenVars[MAX_VARS];
    int countChildren;
    Var *fathers[MAX_VARS];
    int countFathers;
} Var;

typedef struct Data {
    int count;
    Var variables[MAX_VARS];
} Data;

void initData(Data *data);
void initVar(Var *var);
int inData(char *varName, Data *data);
void addVar(char *varName, Data *data);
int inLocalVars(Var *mainVar, Var *desiredVar);
void defineVar(Var* var);
void defineConstants(Data *data);

#endif
