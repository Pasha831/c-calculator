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

// function, that perform the operations in queue
// it is really awful optimized!!!
void doTheOperation(char queue[MAXSIZE][MAXSIZE], const char *op, int *x) {
    int num = 0;
    char snum[MAXSIZE] = { 0 };

    int n1 = 0;  // first number
    int n2 = 0;  // second number
    int k = *x - 1;  // index, used to search numbers in RPN

    // find second number n2 and clean the space in queue[k]
    while (!(queue[k][0] >= '0' && queue[k][0] <= '9')) {
        --k;
    }
    n2 = atoi(queue[k]);
    strcpy(queue[k], snum);

    // find first number n1 and clean the space in queue[k]
    while (!(queue[k][0] >= '0' && queue[k][0] <= '9')) {
        --k;
    }
    n1 = atoi(queue[k]);
    strcpy(queue[k], snum);

    // perform the operation
    if (*op == '*') {
        num = n1 * n2;
    }
    else if (*op == '/') {
        num = n1 / n2;
    }
    else if (*op == '+') {
        num = n1 + n2;
    }
    else if (*op == '-') {
        num = n1 - n2;
    }

    // convert int to string format, 10 - base of a number (decimal)
    itoa(num, snum, 10);

    // place the calculated number in queue instead of operation sign
    strcpy(queue[*x], snum);
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
                    while (k > 0) {  // if stack is not empty
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

        if (z != 0) {  // push the remaining number, if there is one
            strcpy(queue[m++], n);
            cleanNumber(n, &z);
        }
        while (k > 0) {  // push the remaining operations
            strcpy(queue[m++], stack[--k]);
        }

        // calculate RPN (reversed polish notation)
        int x = 0;
        while (x < m) {
            if (inOperators(queue[x])) {
                doTheOperation(queue, queue[x], &x);
            }
            x++;
        }

        // print out the result in output.txt file
        fprintf(fw, "%d\n", atoi(queue[x - 1]));
    }
}
