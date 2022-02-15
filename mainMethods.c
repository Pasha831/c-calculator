#include "mainMethods.h"

// returns an index of an operator from the list or -1 if operator is not in the list
int inOperators(const char *inp) {
    char *operators[] = {"+",
                         "-",
                         "*",
                         "/",
                         "^",
                         "(",
                         ")",
                         "="};
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

// return the priority of each operator
int precedence(const char* op) {
    switch (*op) {
        case '(': case ')':
            return 1;
        case '^':
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
    while (inp[*i] && inOperators(&inp[*i]) == -1 && inp[*i] != ' '){  // reading symbols from input until an operator is found
        str[count++] = inp[(*i)++];
    }
    str[count] = 0; // end of string
    (*i)--;  // step back in input
}

// clean input string from spaces and '\n'
int cleanInput(char* inp){
    char cleanedInput[MAXSIZE*MAXSIZE] = { 0 };
    int count = 0;
    int countBracket[MAXSIZE] = {0};
    int withBracket[MAXSIZE] = {0};
    int countPow = 0;
    int countFunc = 0;
    int isFunc = 0;
    int isPow = 0;
    int isVar = 0;
    int isNum = 0; // it's awful, but lawful
    int dot = 0;
    int start = 0;
    for (int i = 0; inp[i] != '\0' && inp[i] != '\n'; ++i){
        if (count != 0 && (inp[i] >= '0' && inp[i] <= '9') && (cleanedInput[count-1] == ')' || (isVar && inp[i-1] == ' '))) {
            cleanedInput[count++] = '*';
        }

        if ((inp[i] >= '0' && inp[i] <= '9') && (count == 0 || isFunc || inOperators(&cleanedInput[count-1]) != -1)){
            dot = 0;
            isNum = 1;
        }
        else if (isNum) {
            if (inp[i] == '.' || (inp[i] == ',' && countPow == 0)) {
                if (dot) {
                    inp[0] = 0;
                    return 1;
                }
                else {
                    if (inp[i] == ',') {
                        inp[i] = '.';
                    }
                    dot = 1;
                }
            }
            else if (isNum && (inp[i] == '(' || (inp[i] >= 'a' && inp[i] <= 'z') || (inp[i] >= 'A' && inp[i] <= 'Z'))) {
                cleanedInput[count++] = '*';
                isNum = 0;
            }
            else if (isNum && inOperators(&inp[i]) != -1){
                isNum = 0;
            }
        }

        if (isVar  && inOperators(&inp[i]) != -1) {
            if (inp[i] == '(') {
                cleanedInput[count++] = '*';
            }
            isVar = 0;
        }
        if (countFunc && isFunc) {
            if (inp[i] == '(') {
                isFunc = 0;
                withBracket[countFunc] = 1;
                countBracket[countFunc]++;
            } else if (inOperators(&inp[i]) != -1) {
                inp[0] = 0;
                return 2;
            } else if (inp[i] != ' ' && start < i) {
                isFunc = 0;
                withBracket[countFunc] = 0;
                cleanedInput[count++] = '(';
            }
        }
        else if (countFunc) {
            while (countFunc > 0 && countBracket[countFunc] == 0 && inp[i] != '(' && (inp[i] == ')' || inOperators(&inp[i]) != -1)) {
                if (withBracket[countFunc] == 0) {
                    cleanedInput[count++] = ')';
                }
                countFunc--;
            }
            if (inp[i] == '(') {
                countBracket[countFunc]++;
            }
            else if (inp[i] == ')') {
                countBracket[countFunc]--;
            }
        }
        if (countPow && start < i) {
            if (isPow && inp[i] != '(' && inp[i] != ' ') {
                inp[0] = 0;
                return 3;
            }
            else if (isPow && inp[i] == '('){
                isPow = 0;
            }
            else if (inp[i] == ',') {
                cleanedInput[count++] = ')';
                cleanedInput[count++] = '^';
                cleanedInput[count++] = '(';
                i++;
                countPow--;
            }
        }
        else if (!isNum && !countPow && inp[i] == ',') {
            inp[0] = 0;
            return 4;
        }

        if (((inp[i] >= 'a' && inp[i] <= 'z') || (inp[i] >= 'A' && inp[i] <= 'Z')) && (count == 0 || (isVar && inp[i-1] == ' ') || inOperators(&cleanedInput[count-1]) != -1)) {
            start = i;
            char str[MAXSIZE];
            if (isVar) {
                cleanedInput[count++] = '*';
                isVar = 0;
            }
            getSymbols(inp, str, &start);
            if (inFunctions(str) != -1) {
                isFunc = 1;
                countFunc++;
            }
            else if (strcmp(str, "pow") == 0) {
                i = start;
                isPow = 1;
                countPow++;
                continue;
            }
            else {
                isVar = 1;
                if (cleanedInput[count-1] == ')') {
                    cleanedInput[count++] = '*';
                }
            }
        }

        if (inp[i] != ' ') {
            cleanedInput[count++] = inp[i];
        }
    }
    while (countFunc) {
        if (withBracket[countFunc--] == 0) {
            cleanedInput[count++] = ')';
        }
    }

    strcpy(inp, cleanedInput);
    return 0;
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
void createRPN(Var* currentVar, char inp[MAXSIZE], Data *data) {
    char stack[MAXSIZE][MAXSIZE] = { 0 };  // operation stack
    int k = 0;  // pointer for stack

    for (int i = 0; i < strlen(inp); i++) {
        if ((inp[i] >= '0' && inp[i] <= '9') || inp[i] == '.' || inp[i] == 'j') {  // is a part of a digit?
            getNumber(inp, currentVar->polish, &currentVar->countPolish, &i);
        }
        else if (inOperators(&inp[i]) != -1) {
            char operator[MAXSIZE] = { 0 };
            operator[0] = inp[i];

            // possible solution for unary minus
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
                    strcpy(stack[k], "\0");  // when we reached '(': clean '('
                    if (inFunctions(stack[k-1]) != -1){  // if there is a function before '('
                        strcpy(currentVar->polish[currentVar->countPolish++], stack[--k]);  // pop function
                    }
                    break;
                }
                else if (precedence(&inp[i]) < precedence(stack[k - 1]) || stack[k - 1][0] == '(') {  // if previous operator is '(' - add it anyway
                    strcpy(stack[k++], operator);
                    break;
                }
                else if (precedence(&inp[i]) >= precedence(stack[k - 1])) {
                    strcpy(currentVar->polish[currentVar->countPolish++], stack[--k]);
                }
            }

            // empty - fill it! / no need to add ')' at stack
            if (k == 0 && operator[0] != ')') {
                strcpy(stack[k++], operator);
            }
        }
        else {
            char str[MAXSIZE] = { 0 };
            getSymbols(inp, str, &i);

            if (inFunctions(str) != -1){  // if str is a function
                strcpy(stack[k++], str);  // adding to operation stack
            }
            else if (strcmp(str, "pow") != 0) {  // if it's not a function or pow function
                int index = inData(str, data);

                // check for redefinition of variable
                if (currentVar == &data->variables[index]) {
                    currentVar->polish[0][0] = '\0';
                    return;
                }
                else if (index == data->count) {  // if there is no such variable in Data
                    addVar(str, data);  // create it and increment count in Data
                }

                // if it's a variable and there is no such variable in local vars of it
                if (inLocalVars(currentVar, &data->variables[index]) == -1) {
                    currentVar->countUnknown++;
                    currentVar->childrenVars[currentVar->countChildren++] = &data->variables[index];
                    data->variables[index].fathers[data->variables[index].countFathers++] = currentVar;
                }

                // add it anyway to the RPN
                strcpy(currentVar->polish[currentVar->countPolish++], str);
            }
        }
    }

    // push the remaining operations
    while (k > 0) {
        strcpy(currentVar->polish[currentVar->countPolish++], stack[--k]);
    }
}

// function, that performs calculations from reversed polish notation
void calculateRPN(Var* currentVar, Data *data, FILE *fw) {
    // gives the result of calculation in stack[0] after all necessary calculations
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
                                                                 powow,
                                                                 powow};

    // process RPN
    for (int i = 0; i < currentVar->countPolish; ++i){
        if ((currentVar->polish[i][0] >= '0' && currentVar->polish[i][0] <= '9') || currentVar->polish[i][0] == 'j') {
            if (currentVar->polish[i][strlen(currentVar->polish[i])-1] == 'j') {  // if number is imaginary
                stack[count] = strtod(currentVar->polish[i], 0) * I;

                // if string consists only of one 'j'
                if (currentVar->polish[i][0] == 'j') {
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
            stack[count++] = data->variables[inData(currentVar->polish[i], data)].value;
        }
        if (isfinite(creal(stack[count - 1])) == 0 || isfinite(cimag(stack[count - 1])) == 0) {
            zeroDivisionException(fw);
        }
    }

    // returns the value to variable
    currentVar->value = stack[0];
    free(stack);
}

// convenient print of expression
void printAnswer(Var *var, FILE *fw) {
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

// checks whether input line have a '=' sign
int isVariable(char* inp) {
    for (int i = 0; i < strlen(inp); i++) {
        if (inp[i] == '=') {
            return 1;
        }
    }
    return 0;
}

// checking of right bracket sequence
int bracketSequence(char* inp) {
    int count = 0;
    int correct = 1;
    char brackets[MAXSIZE*MAXSIZE] = { 0 };
    for (int i = 0; i < strlen(inp) && correct == 1; ++i){
        switch (inp[i]) {
            case '(': case '[': case '{':
                brackets[count] = inp[i];
                count++;
                break;
            case ')':
                if (count>0 && brackets[count - 1] == '(') {
                    count--;
                } else {
                    correct = 0;
                }
                break;
            case ']':
                if (count>0 && brackets[count - 1] == '[') {
                    count--;
                } else {
                    correct = 0;
                }
                break;
            case '}':
                if (count>0 && brackets[count - 1] == '{') {
                    count--;
                } else {
                    correct = 0;
                }
                break;
        }
    }
    if (correct == 1 && count == 0) {
        for (int i = 0; i < strlen(inp); ++i) {
            switch(inp[i]) {
                case '[': case '{':
                    inp[i] = '(';
                    break;
                case ']': case '}':
                    inp[i] = ')';
                    break;
            }
        }
        return 1;
    }
    return 0;
}

// creates expression name with suitable number in queue of expressions
void createExpressionName(Var *exp, int *countExp) {
    char buff[MAXSIZE];
    char tempExp[MAXSIZE] = "Answer to expression ";
    itoa((*countExp)++, buff, 10);
    strcat(tempExp, buff);
    strcpy(exp->name, tempExp);  // set mainExp name as an Answer
}

// calculate ol RPNs of variables in Data, if it's possible
void calculateAllDataVariables(Data *data, FILE *fw) {
    for (int i = 0; i < data->count; ++i) {
        int flag = 0;  // feature to speed up O(n^2) definition loop

        for (int j = 0; j < data->count; ++j) {
            if (data->variables[j].countUnknown == 0 && data->variables[j].isDefined == 0 && data->variables[j].polish[0][0] != '\0') {
                calculateRPN(&data->variables[j], data, fw);
                defineVar(&data->variables[j]);
                // printAnswer(&data->variables[j], fw);

                flag++;
            }
        }

        // if there were no definitions -> no sense to go further
        if (flag == 0){
            break;
        }
    }
}

void readAndDefineVariables(Data *data, FILE *fr, FILE *fw, char *inp) {
    for (int i = 0; i < data->count - COUNT_CONST && fgets(inp, MAXSIZE*MAXSIZE, fr) && isVariable(inp); i++) {
        cleanInput(inp);  // clean it

        char varName[MAXSIZE] = { 0 };  // variable's name
        int start = 0;  // pointer for changing input to expression
        getSymbols(inp, varName, &start);  // get the name of the variable

        // additional input string for cutting (so that original inp had a "=" sign)
        char tempInp[MAXSIZE];
        strcpy(tempInp, inp);
        toExpression(tempInp, &start);

        // search the position of variable in Data and add it, if there is no such variable
        int pos = inData(varName, data);
        if (pos == data->count){
            addVar(varName, data);
        }

        // checking for redefinition of variable
        if (data->variables[pos].polish[0][0] == '\0') {
            createRPN(&data->variables[pos], tempInp, data);  // create RPN for the variable

            if (data->variables[pos].polish[0][0] == '\0') {
                fprintf(fw, "%s can't be defined properly, because it points to itself.\n", varName);
                --i;
            }
        }
        else {
            fprintf(fw, "%s is already defined.\n", varName);
            --i;
        }
    }
}

void printMainExpression(Var *mainExp, Data *data, FILE *fw, char *inp, char *previousInp, int *countExp) {
    if (mainExp->countUnknown == 0 && mainExp->polish[0][0] != '\0') {
        calculateRPN(mainExp, data, fw);
        fprintf(fw, "Expression interpreted as:\n%s\n", previousInp);
        printAnswer(mainExp, fw);
        inp[0] = 0;
    }
    else {
        // if it is a blank input line
        if (mainExp->polish[0][0] == '\0') {
            --(*countExp);  // we need to increase countExp, because input is an empty line
            return;
        }

        // throw a problem in output
        fprintf(fw, "Expression %d can't be calculated.\n", *countExp-1);

        // if it's already problematic input - let's stop this circus
        if (strcmp(inp, previousInp) == 0) {
            inp[0] = 0;
        }
    }

    if (data->count > COUNT_CONST) {
        fprintf(fw, "Variables used in %d expression:\n", *countExp - 1);
    }
    for (int i = COUNT_CONST; i < data->count; i++) {
        if (data->variables[i].isDefined == 0) {
            fprintf(fw, "Variable %s is not defined.\n", data->variables[i].name);
        }
        else {
            printAnswer(&data->variables[i], fw);
        }
    }

    fprintf(fw, "\n");
}

int errorCheck(FILE *fw, int error) {
    char* errors[5] = {"No errors.",
                       "Superfluous comma or stop in number.",
                       "Function doesn't have any arguments.",
                       "Function <pow> needs brackets." ,
                       "Meaningless comma."};
    if (error != 0) {
        fprintf(fw, "Main expression cannot be interpreted properly.\n");
        fprintf(fw, "%s\n\n", errors[error]);
        return 1;
    }
    return 0;
}
