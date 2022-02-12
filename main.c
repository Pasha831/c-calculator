#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tgmath.h>
#define MAXSIZE 100  // max size of input string

#include "functions.h"
#include "variables.h"

FILE *fr;
FILE *fw;

Data data;  // made ot global, in order to have access to it everywhere in program

// returns an index of an operator from the list or -1 if operator is not in the list
int inOperators(const char *inp) {
    char *operators[] = {"+",
                         "-",
                         "*",
                         "/",
                         "^",
                         ",",  // ^ operator for pow(..., ...)
                         "(",
                         ")",
                         "="};
    for (int i = 0; i < 9; i++) {
        if (*inp == *operators[i]) {
            return i;
        }
    }
    return -1;
}

// returns an index of a function from the list or -1 if function is not in the list
int inFunctions(char* str){
    char* functions[] = { "cos",
                          "sin",
                          "tg",
                          "log",
                          "ln",
                          "sqrt",
                          "abs",
                          "exp",
                          "real",
                          "imag",
                          "mag",
                          "phase"};
    //have to add "real", "imag", "mag", "phase"
    for (int i = 0; i < 12; ++i){
        if (!strcmp(functions[i], str)){
            return i;
        }
    }
    return -1;
}

int precedence(const char* op) {
    switch (*op) {
        case '(': case ')':
            return 1;
        case '^':
            return 2;
        case '*': case '/':
            return 3;
        case '+': case '-': case ',':
            return 4;
        default:
            return -1;
    }
}

// adding new number from input to RPL
void getNumber(char* inp, char polish[MAXSIZE][MAXSIZE], int *m, int *i){
    int count = 0;
    char imag = 0;
    while ((inp[*i] >= '0' && inp[*i] <= '9') || inp[*i] == '.' || inp[*i] == 'j') {  // is a part of a digit?
        if (inp[*i] == 'j'){  // is imaginary?
            imag = 'j';
            (*i)++;
        }
        else {
            polish[*m][count++] = inp[(*i)++];
        }
    }
    polish[*m][count++] = imag;  // adding 'j' to the end of the processed number
    polish[*m][count] = 0;  // end of string
    (*m)++;  // incrementing number of elements of RPN
    (*i)--;  // step back in input
}

// adding symbols (of function or variable) from input to string
void getSymbols(char* inp, char* str, int* i){
    int count = 0;
    while (inp[*i] && inOperators(&inp[*i]) == -1){  // reading symbols from input until an operator is found
        str[count++] = inp[(*i)++];
    }
    str[count] = 0; // end of string
    (*i)--;  // step back in input
}

// clean input string from spaces and '\n'
void cleanInput(char* inp){
    char cleanedInput[MAXSIZE*MAXSIZE] = { 0 };
    int count = 0;

    for (int i = 0; inp[i] != '\0' && inp[i] != '\n'; ++i){
        if (inp[i] == ',') {
            cleanedInput[count++] = ')';
            cleanedInput[count++] = '^';
            cleanedInput[count++] = '(';
        }
        else if (inp[i] != ' ') {
            cleanedInput[count++] = inp[i];
        }
    }

    strcpy(inp, cleanedInput);
}

// deletes variable name and "=" sign from input string to create an expression
void toExpression(char *inp, int *start) {
    int count = 0;
    *start += 2;  // getSymbols points start to the end of the variable name, so we need to jump further for 2 positions
    for (int i = *start; i < strlen(inp); i++) {
        inp[count++] = inp[i];
    }
    inp[count] = '\0';
}

// create RPN using input string
void createRPN(Var* currentVar, char inp[MAXSIZE]) {
    char stack[MAXSIZE][MAXSIZE] = { 0 };  // operation stack
    int k = 0;  // pointer for stack

    for (int i = 0; i < strlen(inp); i++) {
        if ((inp[i] >= '0' && inp[i] <= '9') || inp[i] == '.' || inp[i] == 'j') {  // is a part of a digit?
            getNumber(inp, currentVar->polish, &currentVar->countPolish, &i);
        }
        else if (inOperators(&inp[i]) != -1) {  // is an operator?
            char operator[MAXSIZE] = { 0 };
            operator[0] = inp[i];

            // possible solution for unary minus (does not work with some binary functions)
            if (operator[0] == '-' && (inp[i-1] == '(' || i == 0)){
                strcpy(currentVar->polish[currentVar->countPolish++], "0");
            }

            while (k > 0) {  // if stack is not empty
                if (operator[0] == '(') {  // simply add '(' to stack
                    strcpy(stack[k++], operator);
                    break;
                }
                else if (operator[0] == ')') {  // pop all operators from stack until '('
                    while (strcmp(stack[--k], "(") != 0 && k > 0) {  // while we haven't found '('
                        strcpy(currentVar->polish[currentVar->countPolish++], stack[k]);  // pop operator
                        strcpy(stack[k], "\0");  // clean the position
                    }
                    strcpy(stack[k], "\0");// when we reached '(': clean '('
                    if (inFunctions(stack[k-1]) != -1){  // if there is a function before '('
                        strcpy(currentVar->polish[currentVar->countPolish++], stack[--k]);  // pop function
                    }
                    break;  // then break the search
                }
                else if (precedence(&inp[i]) < precedence(stack[k - 1]) || stack[k - 1][0] == '(') {  // if previous operator is '(' - add it anyway
                    strcpy(stack[k++], operator);
                    break;
                }
                else if (precedence(&inp[i]) >= precedence(stack[k - 1])) {
                    strcpy(currentVar->polish[currentVar->countPolish++], stack[--k]);
                }
            }
            if (k == 0 && operator[0] != ')') {  // no need to add ')' at stack
                strcpy(stack[k++], operator);
            }
        }
        else {
            char str[MAXSIZE] = { 0 };
            getSymbols(inp, str, &i);
            if (inFunctions(str) != -1){  // if str is a function
                strcpy(stack[k++], str);  // adding to operation stack
            }
            else if (strcmp(str, "pow") != 0) {  // if it's not a function or pow function, that we skip
                int index = inData(str, &data);
                if (index == data.count) {  // if there is no such variable in Data
                    addVar(str, &data);  // create it and increment count in Data
                }
                if (inLocalVars(currentVar, &data.variables[index]) == -1) {
                    currentVar->countUnknown++;
                    currentVar->childrenVars[currentVar->countChildren++] = &data.variables[index];
                    data.variables[index].fathers[data.variables[index].countFathers++] = currentVar;
                }
                strcpy(currentVar->polish[currentVar->countPolish++], str);  // add it anyway to the RPN
            }
        }
    }

    while (k > 0) {  // push the remaining operations
        strcpy(currentVar->polish[currentVar->countPolish++], stack[--k]);
    }
}

// function, that performs calculations from reversed polish notation
void calculateRPN(Var* currentVar) {
    double complex* stack = (double complex*)calloc(currentVar->countPolish, sizeof(double complex));
    int count = 0;
    double complex (*func[])(double complex) = {ccos,
                                                csin,
                                                ctan,
                                                clog10,
                                                clog,
                                                csqrt,
                                                cabsd,
                                                cexp,
                                                real,
                                                imag,
                                                cabsd,
                                                phase};
    double complex (*opers[])(double complex, double complex) = {add,
                                                                 subtract,
                                                                 multiply,
                                                                 divide,
                                                                 cpow,
                                                                 cpow};

    for (int i = 0; i < currentVar->countPolish; ++i){
        if ((currentVar->polish[i][0] >= '0' && currentVar->polish[i][0] <= '9') || currentVar->polish[i][0] == 'j'){
            if (currentVar->polish[i][strlen(currentVar->polish[i])-1] == 'j'){ // if number is imaginary
                stack[count] = strtod(currentVar->polish[i], 0) * I;
                if (currentVar->polish[i][0] == 'j'){ // if string consists only of one 'j'
                    stack[count] = I;
                }
                count++;
            }
            else {
                stack[count++] = strtod(currentVar->polish[i], 0);
            }
        }
        else if (inOperators(&currentVar->polish[i][0]) != -1){
            int index = inOperators(&currentVar->polish[i][0]);
            stack[count - 2] = opers[index](stack[count - 2], stack[count - 1]);
            count--;
        }
        else if (inFunctions(currentVar->polish[i]) != -1){
            int index = inFunctions(currentVar->polish[i]);
            stack[count - 1] = func[index](stack[count - 1]);
        }
        else {
            // adding variable value to stack
            stack[count++] = data.variables[inData(currentVar->polish[i], &data)].value;
        }
        if (isfinite(creal(stack[count - 1])) == 0 || isfinite(cimag(stack[count - 1])) == 0) {
            zeroDivisionException(fw);
        }
    }

    currentVar->value = stack[0];
    free(stack);
}

void printAnswer(Var *var) {
    if (creal(var->value) != 0 && cimag(var->value) != 0) {
        if (cimag(var->value) > 0) {
            fprintf(fw, "%s = %g + %gj\n", var->name, creal(var->value), cimag(var->value));
        }
        else {
            fprintf(fw, "%s = %g - %gj\n", var->name, creal(var->value), -1 * cimag(var->value));
        }
    }
    else if (creal(var->value) != 0) {
        fprintf(fw, "%s = %g\n", var->name, creal(var->value));
    }
    else if (cimag(var->value) != 0) {
        fprintf(fw, "%s = %gj\n", var->name, cimag(var->value));
    }
    else {
        fprintf(fw, "%s = %g\n", var->name, creal(var->value));
    }
}


int main() {
    // input here your own files destination
    fr = fopen("D:\\CLionProjects\\c-calculator\\input.txt", "rt");
    fw = fopen("D:\\CLionProjects\\c-calculator\\output.txt", "wt");

    initData(&data);

    char inp[MAXSIZE*MAXSIZE] = { 0 };  // each line of input

    while (fgets(inp, MAXSIZE*MAXSIZE, fr)) {
        Var mainExp;  // main expression
        initVar(&mainExp);
        strcpy(mainExp.name, "Answer");  // set mainExp name as an Answer

        // clean input from spaces and '\n'
        cleanInput(inp);

        // create RPN from input string - the main expression of input!
        createRPN(&mainExp, inp);

        // process "count" numbers, if count = 0 -> it won't launch and everything will be ok!
        fgets(inp, MAXSIZE*MAXSIZE, fr);  // get a new line with variable
        for (int i = 0; i < data.count - 2 && inp[0] != '\0'; i++) {
            cleanInput(inp);  // clean it

            char varName[MAXSIZE] = { 0 };  // variable's name
            int start = 0;  // pointer for changing input to expression
            getSymbols(inp, varName, &start);  // get the name of the variable
            toExpression(inp, &start);

            int pos = inData(varName, &data);  // search the position of variable in Data
            if (pos == data.count && inp[0] != 0){
                addVar(varName, &data);
            }
            createRPN(&data.variables[pos], inp);  // create RPN for the variable

            fgets(inp, MAXSIZE*MAXSIZE, fr);  // get a new line with variable
        }

        defineVar(&data.variables[0]);
        defineVar(&data.variables[1]);
        // calculate RPN for every variable (from the bottom to the top)
        for (int i = 0; i < data.count; ++i) {
            int flag = 0;
            for (int j = 0; j < data.count; ++j) {
                if (data.variables[j].countUnknown == 0 && data.variables[j].isDefined == 0) {
                    flag++;
                    calculateRPN(&data.variables[j]);
                    defineVar(&data.variables[j]);
                    printAnswer(&data.variables[j]);
                }
            }
            if (flag == 0){
                break;
            }
        }

        // calculate RPN (reversed polish notation)
        calculateRPN(&mainExp);

        // print out the result in output.txt file
        printAnswer(&mainExp);
    }
}
