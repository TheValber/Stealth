/*
 * Stealth
 * December 2022 - January 2023
 * BERNIER Valentin - VILAYVANH Mickael
 */

#ifndef __UTIL__
#define __UTIL__

#include <math.h>

/* Value from Wikipedia */
#define PI 3.141592653589793

/**
 * @brief Return the smallest int between a and b
 */
int min(int a, int b);

double min_double(double a, double b);

/**
 * @brief Returns the nearest integer of n
 * 
 */
int round(double n);

/**
 * @brief Return the square of n
 */
double square_double(double n);

/**
 * @brief Return the square root of n
 */
double sqrt_double(double n);

/**
 * @brief Switch the value of a and b
 */
void switch_double(double * a, double * b);

/**
 * @brief Return the sin of x
 */
double sin_double(double x);

/**
 * @brief Return the absolute value of x
 */
double abs_double(double x);

#endif
