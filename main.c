#include "mainMethods.h"
#include "functions.h"

int main() {
    // input here your own files destination
    FILE *fr = fopen("C:\\Users\\ageev\\CLionProject\\c-calculator\\c-calculator\\input.txt", "rt");
    FILE *fw = fopen("C:\\Users\\ageev\\CLionProject\\c-calculator\\c-calculator\\output.txt", "wt");

    Data data;  // storage of variables

    char inp[MAXSIZE*MAXSIZE] = { 0 };  // each line of input
    char previousInp[MAXSIZE*MAXSIZE] = { 0 };  // to store previous line of input
    int countExp = 1;  // number of expressions from user

    // if we have not processed expression - let's process it
    // else - let's read another line of input
    while (inp[0] != 0 || fgets(inp, MAXSIZE*MAXSIZE, fr)) {
        // if input line is not an expression - just skip it
        if (isVariable(inp)) {
            inp[0] = 0;
            continue;
        }

        // new expression - new stored variables
        initData(&data);

        // main expression
        Var mainExp;
        initVar(&mainExp);
        createExpressionName(&mainExp, &countExp);

        // check wrong bracket sequence: calculate if is OK, otherwise continue and clean input
        if (!bracketSequence(inp)) {
            fprintf(fw, "Wrong bracket sequence.\n\n");
            inp[0] = 0;
            continue;
        }

        // clean input from spaces and '\n'
        int error = cleanInput(inp);
        if (errorCheck(fw, error)) {
            continue;
        }

        strcpy(previousInp, inp);
        // create RPN from input expression
        createRPN(&mainExp, inp, &data);

        // read new lines and process new children variables
        readAndDefineVariables(&data, fr, fw, inp);

        // define constants: PI, e
        defineConstants(&data);

        // calculate RPN for every variable
        calculateAllDataVariables(&data, fw);

        // if there is no unknown vars for mainExp - calculate it and print the answer
        printMainExpression(&mainExp, &data, fw, inp, previousInp, &countExp);
    }
}