#ifndef __DATA_MATH__
#define __DATA_MATH__

#include "structs.hpp"

struct data mean_data(struct data* d, const int size); //calcs mean of data array
struct data standard_deviation_data(struct data m, struct data* d, const int size); //calcs stdev of of data array using mean of data array 
float get_speed(float a, float b); //caclulates motion vector
void print_data(struct data d); //prints data in comma separeted line

#endif
