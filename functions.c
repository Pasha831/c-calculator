#include "functions.h"

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