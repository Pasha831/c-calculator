#include "functions.h"

double add(double a, double b){
    return a + b;
}
double subtract(double a, double b){
    return a - b;
}
double multiply(double a, double b){
    return a * b;
}
double divide(double a, double b){
    return a / b;
}

double absd(double x){
    if (x < 0){
        return -x;
    }
    return x;
}