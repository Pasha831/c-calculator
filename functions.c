#include "functions.h"

void zeroDivisionException(FILE *fw) {
    fprintf(fw, "Zero division exception.\n");
    exit(1);
}

double complex add(double complex a, double complex b) {
    return a + b;
}
double complex subtract(double complex a, double complex b) {
    return a - b;
}
double complex multiply(double complex a, double complex b) {
    return a * b;
}
double complex divide(double complex a, double complex b) {
    return a / b;
}

// for type match
double complex cabsd(double complex x) {
    return cabs(x);
}

//log10 for complex numbers
double complex clog10(double complex x) {
    return clog(x)/clog(10);
}

double complex real(double complex x) {
    return creal(x);
}
double complex imag(double complex x) {
    return cimag(x);
}
double complex phase(double complex x) {
    return carg(x);
}

double complex powow(double complex x, double complex y) {
    if (cimag(x) == 0 && cimag(y) == 0 && creal(x) < 0){
        if (cabs(y) < 1){
            return cimag(cpow(x, y))*I;
        }
        else {
            return creal(cpow(x, y));
        }
    }
    else if (creal(x) == 0 && cimag(y) == 0 && creal(y) == (int)creal(y)){
        if ((int)creal(y) % 2 == 0){
            return creal(cpow(x, y));
        }
        else if ((int)creal(y) % 2 != 0){
            return cimag(cpow(x, y))*I;
        }
    }
    return cpow(x, y);
}