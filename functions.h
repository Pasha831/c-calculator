#include <tgmath.h>

#ifndef C_CALCULATOR_FUNCTIONS_H
#define C_CALCULATOR_FUNCTIONS_H

double complex add(double complex a, double complex b);
double complex subtract(double complex a, double complex b);
double complex multiply(double complex a, double complex b);
double complex divide(double complex a, double complex b);
double complex cabsd(double complex x);
double complex clog10(double complex x);
double complex real(double complex x);
double complex imag(double complex x);
double complex phase(double complex x);

#endif //C_CALCULATOR_FUNCTIONS_H
