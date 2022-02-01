#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAXSIZE 100  // max size of input string

FILE *fr;
FILE *fw;

// abs function for double (absolute value), (not working, because we don't work with unary minus)
double absd(double x){
    if (x < 0){
        return -x;
    }
    return x;
}

// is an operator?
int inOperators(const char *inp) {
    // we have to add other operators, like: '(', ')'
    if (*inp == '+' || *inp == '-' || *inp == '*' || *inp == '/' || *inp == '^' || *inp == '(' || *inp == ')') {
        return 1;
    }
    else {
        return 0;
    }
}

// adding new number from input to RPL
void getNumber(char* inp, char polish[MAXSIZE][MAXSIZE], int *m, int *i){
    int count = 0;
    while ((inp[*i] >= '0' && inp[*i] <= '9') || inp[*i] == '.' || inp[*i] == ',') {  // is a part of a digit?
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
    while (!inOperators(&inp[*i])){  // reading symbols from input until an operator is found
        str[count++] = inp[(*i)++];
    }
    str[count] = 0;
    (*i)--;
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
    //have to add "pow", "real", "imag", "mag", "phase"
    for (int i = 0; i < 8; ++i){
        if (!strcmp(functions[i], str)){
            return i;
        }
    }
    return -1;
}

/* return the precedence (priority) of operator:
    '(', ')' -> 1
    '^' -> 2
    '*', '/' -> 3
    '+', '-' -> 4
*/
int precedence(const char* op) {
    if (*op == '(' || *op == ')') {
        return 1;
    }
    else if (*op == '^') {
        return 2;
    }
    else if (*op == '*' || *op == '/') {
        return 3;
    }
    else if (*op == '+' || *op == '-') {
        return 4;
    }
}

// create RPN using input string
void createRPN(char polish[MAXSIZE][MAXSIZE], int *m, char inp[MAXSIZE]) {
    char stack[MAXSIZE][MAXSIZE] = { 0 };  // operation stack
    int k = 0;  // pointer for stack

    for (int i = 0; i < strlen(inp); i++) {
        if ((inp[i] >= '0' && inp[i] <= '9') || inp[i] == '.' || inp[i] == ',') {  // is a part of a digit?
            getNumber(inp, polish, m, &i);
        }
        else if (inOperators(&inp[i])) {  // is an operator?
            char operator[MAXSIZE] = { 0 };
            operator[0] = inp[i];


            if (k == 0) {  // if stack is empty
                strcpy(stack[k++], operator);
            }
            else {
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
        }
        else{
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

    for (int i = 0; i < n; ++i){
        if (polish[i][0] >= '0' && polish[i][0] <= '9'){
            stack[count] = strtod(polish[i], 0);
            count++;
        }
        else if (inOperators(&polish[i][0])){
            if (polish[i][0] == '+') {
                stack[count - 2] += stack[count - 1];
            } else if (polish[i][0] == '-') {
                stack[count - 2] -= stack[count - 1];
            } else if (polish[i][0] == '*') {
                stack[count - 2] *= stack[count - 1];
            } else if (polish[i][0] == '/') {
                stack[count - 2] /= stack[count - 1];
            } else if (polish[i][0] == '^') {
                stack[count - 2] = pow(stack[count - 2], stack[count - 1]);
            }
            stack[count - 1] = 0;
            count--;
        }
        else {
            int index = inFunctions(polish[i]);
            if (index != -1) {
                stack[count - 1] = func[index](stack[count - 1]);
            }
        }
    }

    double res = stack[0];
    free(stack);
    return res;
}


int main() {
    // input here your own files destination
    fr = fopen("C:\\Users\\ageev\\CLionProject\\c-calculator\\c-calculator\\input.txt", "rt");
    fw = fopen("C:\\Users\\ageev\\CLionProject\\c-calculator\\c-calculator\\output.txt", "wt");

    char inp[MAXSIZE] = { 0 };  // each line of input

    while (fgets(inp, MAXSIZE, fr)) {
        char polish[MAXSIZE][MAXSIZE] = { 0 };  // reversed polish notation
        int m = 0;

        // create RPN from input string
        createRPN(polish, &m, inp);

        // calculate RPN (reversed polish notation)
        double res = calculateRPN(polish, m);

        // print out the result in output.txt file
        fprintf(fw, "%lf\n", res);
    }
}
