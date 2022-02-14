#include <tgmath.h>
#include <stdlib.h>
#include <stdio.h>
#define EPS 1e-15

#ifndef C_CALCULATOR_FUNCTIONS_H
#define C_CALCULATOR_FUNCTIONS_H

void zeroDivisionException(FILE *fw);
double complex add(double complex a, double complex b);
double complex subtract(double complex a, double complex b);
double complex multiply(double complex a, double complex b);
double complex divide(double complex a, double complex b);
double complex cabsd(double complex x);
double complex clog10(double complex x);
double complex real(double complex x);
double complex imag(double complex x);
double complex phase(double complex x);
double complex powow(double complex x, double complex y);
#endif //C_CALCULATOR_FUNCTIONS_H
