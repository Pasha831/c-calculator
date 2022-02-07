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

    // initialize empty RPN
    var->countPolish = 0;
    var->countLocal = 0;
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

int inLocalVars(Var *mainVar, Var *desiredVar) {
    for (int i = 0; i < mainVar->countLocal; ++i){
        if (mainVar->localVars[i] == desiredVar){
            return i;
        }
    }
    return -1;
}

void defineVar(Var* var) {
    var->isDefined = 1;
    for (int i = 0; i < var->countLinked; ++i){
        var->linked[i]->unknown--;
    }
}