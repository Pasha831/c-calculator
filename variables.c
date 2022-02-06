#include "variables.h"
#include <stdlib.h>
#include <tgmath.h>
#include <string.h>
#define MAXSIZE 100
#define MAX_VARS 10

void initData(Data *data) {
    data->count = 0;
    for (int i = 0; i < MAX_VARS; i++) {
        initVar(&data->variables[i]);
    }
}

void initVar(Var *var) {
    var->name[0] = '\0';  // empty string
    var->value = 0;
    var->isInit = 0;

    // initialize empty RPN
    var->m = 0;
    for (int i = 0; i < MAXSIZE; i++) {
        var->polish[i][0] = '\0';
    }

    var->unknown = 0;
    var->linked = calloc(MAX_VARS, sizeof (Var));
}

int inData(char *varName, Data *data) {
    for (int i = 0; i < MAX_VARS; i++) {
        if (data->variables[i].name[0] == '\0') {
            return -1;  // if we found '\0', there is no sense to go further
        }
        else if (strcmp(data->variables[i].name, varName) == 0) {
            return i;  // return exact place in the Data array
        }
    }
    return -1;  // just in case :)
}

void addVar(char *varName, Data *data) {
    for (int i = 0; i < MAX_VARS; i++) {
        if (data->variables[i].name[0] == '\0') {
            strcpy(data->variables[i].name, varName);  // add the name
            data->count++;  // increment the number of variables
            return;  // exit the loop
        }
    }
}