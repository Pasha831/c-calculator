#include "variables.h"
#include <string.h>
#define MAXSIZE 100
#define MAX_VARS 10

void initData(Data *data) {
    data->count = 0;
}

void initVar(Var *var) {
    var->name[0] = '\0';  // empty string
    var->value = 0;
    var->isInit = 0;

    // initialize empty RPN
    var->countPolish = 0;
    for (int i = 0; i < MAXSIZE; i++) {
        var->polish[i][0] = '\0';
    }

    var->unknown = 0;
    var->countLinked = 0;
}

int inData(char *varName, Data *data) {
    for (int i = 0; i < data->count; i++) {
        if (strcmp(data->variables[i].name, varName) == 0) {
            return i;  // return exact place in the Data array
        }
    }
    return data->count;  // if not in data, return current number of variables on data
}

void addVar(char *varName, Data *data) {
    initVar(&data->variables[data->count]);  // initialize a variable
    strcpy(data->variables[data->count++].name, varName);  // add the name
}