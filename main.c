#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAXSIZE 100  // max size of input string

#include "functions.h"

FILE *fr;
FILE *fw;

// returns an index of an operator from the list or -1 if operator is not in the list
int inOperators(const char *inp) {
    char *operators[] = {"+",
                         "-",
                         "*",
                         "/",
                         "^",
                         ",",  // ^ operator for pow(..., ...)
                         "(",
                         ")"};
    for (int i = 0; i < 8; i++) {
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
                          "exp"};
    //have to add "real", "imag", "mag", "phase"
    for (int i = 0; i < 8; ++i){
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
        case '^': case ',':
            return 2;
        case '*': case '/':
            return 3;
        case '+': case '-':
            return 4;
        default:
            return -1;
    }
}

// adding new number from input to RPL
void getNumber(char* inp, char polish[MAXSIZE][MAXSIZE], int *m, int *i){
    int count = 0;
    while ((inp[*i] >= '0' && inp[*i] <= '9') || inp[*i] == '.') {  // is a part of a digit?
        if (inp[*i] == ','){
            inp[*i] = '.';
        }
        polish[*m][count++] = inp[(*i)++];
    }
    polish[*m][count] = 0;
    (*m)++;
    (*i)--;
}

// adding symbols (of function or variable) from input to string
void getSymbols(char* inp, char* str, int* i){
    int count = 0;
    while (inOperators(&inp[*i]) == -1){  // reading symbols from input until an operator is found
        str[count++] = inp[(*i)++];
    }
    str[count] = 0;
    (*i)--;
}

// clean input string from spaces and '\n'
void cleanInput(char* inp){
    int count = 0;
    for (int i = 0; inp[i] != '\0' && inp[i] != '\n'; ++i){
        if (inp[i] != ' '){
            inp[count++] = inp[i];
        }
    }
    inp[count] = 0;
}

// create RPN using input string
void createRPN(char polish[MAXSIZE][MAXSIZE], int *m, char inp[MAXSIZE]) {
    char stack[MAXSIZE][MAXSIZE] = { 0 };  // operation stack
    int k = 0;  // pointer for stack

    for (int i = 0; i < strlen(inp); i++) {
        if ((inp[i] >= '0' && inp[i] <= '9') || inp[i] == '.') {  // is a part of a digit?
            getNumber(inp, polish, m, &i);
        }
        else if (inOperators(&inp[i]) != -1) {  // is an operator?
            char operator[MAXSIZE] = { 0 };
            operator[0] = inp[i];

            // possible solution for unary minus (does not work with some binary functions)
            if (operator[0] == '-' && (inp[i-1] == '(' || i == 0)){
                strcpy(polish[(*m)++], "0");
            }

            while (k > 0) {  // if stack is not empty
                if (operator[0] == '(') {  // simply add '(' to stack
                    strcpy(stack[k++], operator);
                    break;
                }
                else if (operator[0] == ')') {  // pop all operators from stack until '('
                    while (strcmp(stack[--k], "(") != 0 && k > 0) {  // while we haven't found '('
                        strcpy(polish[(*m)++], stack[k]);  // pop operator
                        strcpy(stack[k], "\0");  // clean the position
                    }
                    strcpy(stack[k], "\0");// when we reached '(': clean '('
                    if (inFunctions(stack[k-1]) != -1){  // if there is a function before '('
                        strcpy(polish[(*m)++], stack[--k]);  // pop function
                    }
                    break;  // then break the search
                }
                else if (precedence(&inp[i]) < precedence(stack[k - 1]) || stack[k - 1][0] == '(') {  // if previous operator is '(' - add it anyway
                    strcpy(stack[k++], operator);
                    break;
                }
                else if (precedence(&inp[i]) >= precedence(stack[k - 1])) {
                    strcpy(polish[(*m)++], stack[--k]);
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
        }
    }

    while (k > 0) {  // push the remaining operations
        strcpy(polish[(*m)++], stack[--k]);
    }
}

// function, that performs calculations from reversed polish notation
double calculateRPN(char polish[MAXSIZE][MAXSIZE], int n) {
    double* stack = (double*)calloc(n, sizeof(double));
    int count = 0;
    double (*func[])(double) = {cos,
                                sin,
                                tan,
                                log10,
                                log,
                                sqrt,
                                absd,
                                exp};
    double (*opers[])(double, double) = {add,
                                         subtract,
                                         multiply,
                                         divide,
                                         pow,
                                         pow};

    for (int i = 0; i < n; ++i){
        if (polish[i][0] >= '0' && polish[i][0] <= '9'){
            stack[count] = strtod(polish[i], 0);
            count++;
        }
        else if (inOperators(&polish[i][0]) != -1){
            int index = inOperators(&polish[i][0]);
            stack[count - 2] = opers[index](stack[count - 2], stack[count - 1]);
            count--;
        }
        else if (inFunctions(polish[i]) != -1){
            int index = inFunctions(polish[i]);
            stack[count - 1] = func[index](stack[count - 1]);
        }
    }

    double res = stack[0];
    free(stack);
    return res;
}


int main() {
    // input here your own files destination
    fr = fopen("C:\\Users\\medve\\CLionProject\\c-calculator\\c-calculator\\input.txt", "rt");
    fw = fopen("C:\\Users\\medve\\CLionProject\\c-calculator\\c-calculator\\output.txt", "wt");

    char inp[MAXSIZE*MAXSIZE] = { 0 };  // each line of input

    while (fgets(inp, MAXSIZE*MAXSIZE, fr)) {
        char polish[MAXSIZE][MAXSIZE] = { 0 };  // reversed polish notation
        int m = 0;

        // clean input from spaces and '\n'
        cleanInput(inp);

        // create RPN from input string
        createRPN(polish, &m, inp);

        // calculate RPN (reversed polish notation)
        double res = calculateRPN(polish, m);

        // print out the result in output.txt file
        fprintf(fw, "%lf\n", res);
    }
}
