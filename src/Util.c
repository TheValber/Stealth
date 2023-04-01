/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#include "../include/Util.h"

int min(int a, int b) {
    return (a <= b) ? a : b;
}

double min_double(double a, double b) {
    return (a <= b) ? a : b;
}

int round(double n){
    return (n < 0.5) ? (int) n : (int) n + 1;
}

double square_double(double n) {
    return n * n;
}

double sqrt_double(double n) {
    return sqrt(n);
}

void switch_double(double * a, double * b) {
    double tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

double sin_double(double x) {
    return sin(x);
}

double abs_double(double x) {
    return x >= 0 ? x : -x;
}
