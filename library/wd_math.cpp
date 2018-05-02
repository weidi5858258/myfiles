#include <stdio.h>
#include <math.h>

#define PI 3.14159265

/***
 double cos(double x);
 */
void wd_cos(void){
    double param, result;
    param = 60.0;
    result = cos(param * PI / 180.0);
    printf("The cosine of %f degrees is %f.\n", param, result);
}

/***
 double sin(double x);
 */
void wd_sin(void){
    double param, result;
    param = 30.0;
    result = sin(param * PI / 180);
    printf("The sine of %f degrees is %f.\n", param, result);
}

/***
 double tan(double x);
 */
void wd_tan(void){
    double param, result;
    param = 45.0;
    result = tan(param * PI / 180.0);
    printf("The tangent of %f degrees is %f.\n", param, result);
}

























