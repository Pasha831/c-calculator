#ifndef C_CALCULATOR_MAINMETHODS_H
#define C_CALCULATOR_MAINMETHODS_H

#define MAXSIZE 100

#include "variables.h"
#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int inOperators(const char *inp);
int inFunctions(char* str);
int precedence(const char* op);
void getNumber(char* inp, char polish[MAXSIZE][MAXSIZE], int *m, int *i);
void getSymbols(char* inp, char* str, int* i);
void cleanInput(char* inp);
void toExpression(char *inp, int *start);
void createRPN(Var* currentVar, char inp[MAXSIZE], Data *data);
void calculateRPN(Var* currentVar, Data *data, FILE *fw);
void printAnswer(Var *var, FILE *fw);
void printMainExpression(Var *mainExp, Data *data, FILE *fw, char *inp, char *previousInp, int *countExp);
int isVariable(char* inp);
int bracketSequence(char* inp);
void createExpressionName(Var *exp, int *countExp);
void calculateAllDataVariables(Data *data, FILE *fw);
void readAndDefineVariables(Data *data, FILE *fr, FILE *fw, char *inp);

#endif //C_CALCULATOR_MAINMETHODS_H
