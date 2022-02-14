#include "mainMethods.h"
#include "functions.h"
#include "variables.h"

int main() {
    // input here your own files destination
    FILE *fr = fopen("D:\\CLionProjects\\c-calculator\\input.txt", "rt");
    FILE *fw = fopen("D:\\CLionProjects\\c-calculator\\output.txt", "wt");

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

        // clean input from spaces and '\n' and copy it
        cleanInput(inp);
        strcpy(previousInp, inp);

        // check wrong bracket sequence: calculate if is OK, otherwise continue and clean input
        if (!bracketSequence(inp)) {
            fprintf(fw, "Wrong bracket sequence or extra comma.\n");
            inp[0] = 0;
            continue;
        }

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