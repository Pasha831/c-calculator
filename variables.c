#include "variables.h"
#include <string.h>

void initData(Data *data) {
    data->count = 0;
    addVar("PI", data);
    data->variables[data->count-1].value = M_PI;
    data->variables[data->count-1].isDefined = 1;
    addVar("e", data);
    data->variables[data->count-1].value = M_E;
    data->variables[data->count-1].isDefined = 1;
}

void initVar(Var *var) {
    var->name[0] = '\0';  // empty string
    var->value = 0;
    var->isDefined = 0;

    var->countPolish = 0;
    var->countChildren = 0;
    var->countFathers = 0;

    var->countUnknown = 0;
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

int inLocalVars(Var *mainVar, Var *desiredVar) {
    for (int i = 0; i < mainVar->countChildren; ++i){
        if (mainVar->childrenVars[i] == desiredVar){
            return i;
        }
    }
    return -1;
}

void defineVar(Var* var) {
    var->isDefined = 1;
    for (int i = 0; i < var->countFathers; ++i){
        var->fathers[i]->countUnknown--;
    }
}