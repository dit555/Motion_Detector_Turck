#ifndef __CAR__
#define __CAR__

#include <iostream>
#include <string>
#include <fstream>

#include "structs.hpp"


class Car{
public:
	Car(std::string input_file_path, std::string output_file_path); //constructor input filepath name
	struct data tokenize_line(); //read next line from file and separate data into data struct
	void update_car_speed(struct data d); //updates speed of car
	void write_to_output(float time, int flag); //writes data to new csv
	void run_moving(); //reads the file and decides whether moving
	~Car(); //closes the files

private:
	std::ifstream car_data_file; //stores opened file
	std::ofstream car_output_file; //stores new csv file
	int eof_flag; //end of file flag

	//current car data
	float car_time_previous;
	float car_speed_Y;
	float car_speed_Z;
	float car_rotation_speed_X;

	//zeroing data
	struct data car_mean; // holds the mean of the data
	struct data car_standard_deviation; // holds the stdev of the data
};

#endif
