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
    while (inOperators(&inp[*i]) == -1){  // reading symbols from input until an operator is found
        str[count++] = inp[(*i)++];
    }
    str[count] = 0; // end of string
    (*i)--;  // step back in input
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

// deletes variable name and "=" sign from input string to create an expression
void toExpression(char *inp, int *start) {
    int count = 0;
    *start += 2;  // getSymbols points start to the end of the variable name, so we need to jump further for 2 positions
    for (int i = *start; i < strlen(inp); i++) {
        inp[count++] = inp[i];
    }
    inp[count++] = '\0';
}

// create RPN using input string
void createRPN(char polish[MAXSIZE][MAXSIZE], int *m, char inp[MAXSIZE]) {
    char stack[MAXSIZE][MAXSIZE] = { 0 };  // operation stack
    int k = 0;  // pointer for stack

    for (int i = 0; i < strlen(inp); i++) {
        if ((inp[i] >= '0' && inp[i] <= '9') || inp[i] == '.' || inp[i] == 'j') {  // is a part of a digit?
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
            else if (strcmp(str, "pow") != 0) {  // if it's not a function or pow function, that we skip
                if (inData(str, &data) == -1) {  // if there is no such variable in Data
                    addVar(str, &data);  // create it and increment count in Data
                }
                strcpy(polish[(*m)++], str);  // add it anyway to the RPN
            }
        }
    }

    while (k > 0) {  // push the remaining operations
        strcpy(polish[(*m)++], stack[--k]);
    }
}

// function, that performs calculations from reversed polish notation
double complex calculateRPN(char polish[MAXSIZE][MAXSIZE], int n) {
    double complex* stack = (double complex*)calloc(n, sizeof(double complex));
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

    for (int i = 0; i < n; ++i){
        if ((polish[i][0] >= '0' && polish[i][0] <= '9') || polish[i][0] == 'j'){
            if (polish[i][strlen(polish[i])-1] == 'j'){ // if number is imaginary
                stack[count] = strtod(polish[i], 0) * I;
                if (polish[i][0] == 'j'){ // if string consists only of one 'j'
                    stack[count] = I;
                }
                count++;
            }
            else {
                stack[count++] = strtod(polish[i], 0);
            }
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

    double complex res = stack[0];
    free(stack);
    return res;
}


int main() {
    // input here your own files destination
    fr = fopen("C:\\Users\\ageev\\CLionProject\\c-calculator\\c-calculator\\input.txt", "rt");
    fw = fopen("C:\\Users\\ageev\\CLionProject\\c-calculator\\c-calculator\\output.txt", "wt");

    initData(&data);

    char inp[MAXSIZE*MAXSIZE] = { 0 };  // each line of input

    while (fgets(inp, MAXSIZE*MAXSIZE, fr)) {
        char polish[MAXSIZE][MAXSIZE] = { 0 };  // reversed polish notation
        int m = 0;

        // clean input from spaces and '\n'
        cleanInput(inp);

        // create RPN from input string - the main expression of input!
        createRPN(polish, &m, inp);

        // process "count" numbers, if count = 0 -> it won't launch and everything will be ok!
        for (int i = 0; i < data.count; i++) {
            fgets(inp, MAXSIZE*MAXSIZE, fr);  // get a new line with variable
            cleanInput(inp);  // clean it

            char varName[MAXSIZE] = { 0 };  // variable's name
            int start = 0;  // pointer for changing input to expression
            getSymbols(inp, varName, &start);  // get the name of the variable
            toExpression(inp, &start);

            int pos = inData(varName, &data);  // search the position of variable in Data
            createRPN(data.variables[pos].polish, &data.variables[pos].m, inp);  // create RPN for the variable
        }


        // calculate RPN (reversed polish notation)
        double complex res = calculateRPN(polish, m);

        // print out the result in output.txt file
        fprintf(fw, "Answer: %.3f + %.3fj\n", creal(res), cimag(res));
    }
}
