#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 1000  // max size of input string

FILE *fr;
FILE *fw;

// is an operator?
int inOperators(const char *inp) {
    // we have to add other operators, like: '(', ')', '^'
    if (*inp == '+' || *inp == '-' || *inp == '*' || *inp == '/') {
        return 1;
    }
    else {
        return 0;
    }
}

// clean the number string and set z (pointer) to 0
void cleanNumber(char *n, int *z) {
    for (int i = 0; i < *z; i++) {
        n[i] = '\0';
    }
    *z = 0;
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


int main() {
    // input here your own files destination
    fr = fopen("C:\\Users\\medve\\CLionProjects\\c-calc\\c-calculator\\input.txt", "r");
    fw = fopen("C:\\Users\\medve\\CLionProjects\\c-calc\\c-calculator\\output.txt", "w");

    char inp[MAXSIZE] = { 0 };  // each line of input

    char stack[MAXSIZE][MAXSIZE] = { 0 };
    char queue[MAXSIZE][MAXSIZE] = { 0 };
    int k = 0;  // pointer for stack
    int m = 0;  // pointer for queue

    char n[MAXSIZE] = { 0 };  // to store long numbers from input
    int z = 0;  // pointer for number

    while (fgets(inp, MAXSIZE, fr)) {
        for (int i = 0; i < strlen(inp); i++) {
            if (inp[i] >= '0' && inp[i] <= '9') {  // is a digit?
                n[z++] = inp[i];
            }
            else if (inOperators(&inp[i])) {  // is an operator?
                char operator = inp[i];

                if (z != 0) {  // if number is not empty
                    strcpy(queue[m++], n);
                    cleanNumber(n, &z);
                }

                if (k == 0) {  // if stack is empty
                    strcpy(stack[k++], &operator);
                }
                else {
                    while (k > 0) {
                        if (precedence(&inp[i]) < precedence(stack[k - 1])) {
                            strcpy(stack[k++], &operator);
                            break;
                        }
                        else if (precedence(&inp[i]) >= precedence(stack[k - 1])) {
                            strcpy(queue[m++], stack[--k]);
                        }
                    }
                    if (k == 0) {  // need optimization XD
                        strcpy(stack[k++], &operator);
                    }
                }
            }
        }

        if (z != 0) {  // if number is not empty
            strcpy(queue[m++], n);
            cleanNumber(n, &z);
        }
        while (k >= 0) {
            strcpy(queue[m++], stack[--k]);
        }
    }
}
