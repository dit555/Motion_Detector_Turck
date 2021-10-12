#include <math.h>
#include <iostream>
#include "../header/structs.hpp"


struct data mean_data(struct data* d, const int size){
	struct data m{0,0,0,0};
	for (int i = 0; i < size; i++){
		m.accel_Y += d[i].accel_Y;
		m.accel_Z += d[i].accel_Z;
		m.rot_speed_X += d[i].rot_speed_X;
	}
	m.accel_Y /= (float)size;
	m.accel_Z /= (float)size;
	m.rot_speed_X /= (float)size;
	return m;
}

struct data standard_deviation_data(struct data m, struct data* d, const int size){
	struct data s{0,0,0,0};
	for (int i = 0; i < size; i++){
		s.accel_Y += pow(d[i].accel_Y - m.accel_Y, 2);
		s.accel_Z += pow(d[i].accel_Z - m.accel_Z, 2);
		s.rot_speed_X += pow(d[i].rot_speed_X - m.rot_speed_X, 2);
	}
	s.accel_Y /= (float)size - 1;
	s.accel_Z /= (float)size - 1;
	s.rot_speed_X /= (float)size - 1;
	s.accel_Y = sqrt(s.accel_Y);
	s.accel_Z = sqrt(s.accel_Z);
	s.rot_speed_X = sqrt(s.rot_speed_X);


	return s;
}

float get_speed(float a, float b){
	float c = a * a + b * b;
	c = sqrt(c);
	return c;
}

void print_data(struct data d){
	std::cout << d.timestamp << "," << d.accel_Y << "," << d.accel_Z << "," << d.rot_speed_X << std::endl;	
}
